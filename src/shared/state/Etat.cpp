#include "state.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>
#include <vector>

using namespace state;
using namespace std;

int Etat::initCurseur(){
	ptr_curseur=nullptr;
	ptr_curseur=new Curseur();
	return 1;
}

Etat::~Etat(){
	delete ptr_curseur;
	ptr_curseur=nullptr;
	
}

int Etat::verifStatut(){
	int selec=-1;
	for(size_t i=0; i<personnages.size(); i++){
		if(personnages[i]->getStatut()==SELECTIONNE){
			selec=i;
			break;
		}
	}
	return selec;
}

vector<vector<unique_ptr<Terrain>>>& Etat::getGrille(){
	vector<vector<unique_ptr<Terrain>>> & refGrille = grille;
	return refGrille;
}

vector<unique_ptr<Personnage>>& Etat::getPersonnages(){
	vector<unique_ptr<Personnage>> & refPersonnages = personnages;
	return refPersonnages;
}

Curseur* Etat::getCurseur(){
	return ptr_curseur;
}

int Etat::getTour(){
	return tour;
}

void Etat::setTour(int newTour){
	tour=newTour;
}

int Etat::initGrille (std::string chemin_map_txt, unsigned int longueur, unsigned int largeur, Correspondances& correspondances){
		
    std::ifstream fichier(chemin_map_txt, ios::in);    
    std::string contenu, ligne, code_tuile;
    int map_tuiles_code[longueur*largeur];
    
    // Lecture Fichier
    if (fichier){
    	while (getline(fichier,ligne)){
			ligne = ligne + ",";
			contenu = contenu + ligne;
		}
		fichier.close();
    }
    else {return -1;}
    
    // Conversion des codes des tuiles en int
    std::stringstream contenuStream(contenu);
    unsigned int i = 0, j = 0, k = 0;
    
    while(std::getline(contenuStream, code_tuile, ',')){
    	map_tuiles_code[i] = std::stoi(code_tuile);
    	i++;
    }
    
    // Remplissage de la grille de jeu avec les terrains
    for (i = 0; i < largeur; i++){
    	std::vector<std::unique_ptr<Terrain>> newLigne;
    
    	for (j = 0; j < longueur; j++){
    		if (map_tuiles_code[k] >= 0 && map_tuiles_code[k] <= 46){
			    // On regarde si le code de la tuile est celui d'un Terrain Praticable
				if (correspondances.getCorrespondanceTP().find(map_tuiles_code[k]) != correspondances.getCorrespondanceTP().end()){
					TerrainPraticable newTP(correspondances.getCorrespondanceTP()[map_tuiles_code[k]],i,j,map_tuiles_code[k]);
					std::unique_ptr<TerrainPraticable> ptr(new TerrainPraticable(newTP)) ;
					newLigne.push_back(move(ptr));
				}
				// Cas du Terrain Non Praticable
				else if (correspondances.getCorrespondanceTNP().find(map_tuiles_code[k]) != correspondances.getCorrespondanceTNP().end()){
					TerrainNonPraticable newTNP(correspondances.getCorrespondanceTNP()[map_tuiles_code[k]],i,j,map_tuiles_code[k]);
					std::unique_ptr<TerrainNonPraticable> ptr(new TerrainNonPraticable(newTNP)) ;
					newLigne.push_back(move(ptr));
				}
				
			}
    		else{
    			cerr << "Code Tuile " << map_tuiles_code[k]<< " invalide dans le fichier " << chemin_map_txt << endl;
    			return 0;
    		
    		}
    		
    		k++;
    	}
    	grille.push_back(move(newLigne));
    }    
    return 1;
}

int Etat::initPersonnages(Correspondances& correspondance){

	 Personnage archer_bleu(ARCHER, true, "Oliver", 1, 21, 0);
	 std::unique_ptr<Personnage> ptrAB(new Personnage(archer_bleu));
	 personnages.push_back(move(ptrAB));
	 
	 Personnage brigand_bleu(BRIGAND, true, "Jason", 2, 20, 1);
	 std::unique_ptr<Personnage> ptrBB(new Personnage(brigand_bleu));
	 personnages.push_back(move(ptrBB));
	 
	 Personnage chevalier_bleu(CHEVALIER, true, "Bruce", 3, 23, 2);
	 std::unique_ptr<Personnage> ptrCB(new Personnage(chevalier_bleu));
	 personnages.push_back(move(ptrCB));
	 
	 Personnage guerrier_bleu(GUERRIER, true, "Arthur", 4, 22, 3);
	 std::unique_ptr<Personnage> ptrGB(new Personnage(guerrier_bleu));
	 personnages.push_back(move(ptrGB));
	 
	 Personnage archer_rouge(ARCHER, false, "Clint", 17, 5, 4);
	 std::unique_ptr<Personnage> ptrAR(new Personnage(archer_rouge));
	 personnages.push_back(move(ptrAR));
	 
	 Personnage brigand_rouge(BRIGAND, false, "Peter", 20, 2, 5);
	 std::unique_ptr<Personnage> ptrBR(new Personnage(brigand_rouge));
	 personnages.push_back(move(ptrBR));
	 
	 Personnage chevalier_rouge(CHEVALIER, false, "Tony", 23, 3, 6);
	 std::unique_ptr<Personnage> ptrCR(new Personnage(chevalier_rouge));
	 personnages.push_back(move(ptrCR));
	 
	 Personnage guerrier_rouge(GUERRIER, false, "Steve", 3, 21, 7);
	 std::unique_ptr<Personnage> ptrGR(new Personnage(guerrier_rouge));
	 personnages.push_back(move(ptrGR));
	
	return 1;
}

void Etat::setFin(bool resultat){
	fin = resultat;
}

bool Etat::getFin(){
	return fin;
}
