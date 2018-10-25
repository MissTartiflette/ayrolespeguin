#include "state.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>
#include <vector>

using namespace state;
using namespace std;

std::vector<std::vector<std::unique_ptr<Terrain>>>& Etat::getGrille(){
	std::vector<std::vector<std::unique_ptr<Terrain>>> & refGrille = grille;
	return refGrille;
}

std::vector<std::unique_ptr<Personnage>>& Etat::getPersonnages(){
	std::vector<std::unique_ptr<Personnage>> & refPersonnages = personnages;
	return refPersonnages;
}

int Etat::getTour(){
	return tour;
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
    for (i = 0; i < longueur; i++){
    	std::vector<std::unique_ptr<Terrain>> newLigne;
    
    	for (j = 0; j < largeur; j++){
    		if (map_tuiles_code[k] >= 0 && map_tuiles_code[k] <= 46){
			    // On regarde si le code de la tuile est celui d'un Terrain Praticable
				if (correspondances.getCorrespondanceTP().find(map_tuiles_code[k]) != correspondances.getCorrespondanceTP().end()){
					TerrainPraticable newTP(correspondances.getCorrespondanceTP()[map_tuiles_code[k]],j,i,map_tuiles_code[k]);
					std::unique_ptr<TerrainPraticable> ptr(new TerrainPraticable(newTP)) ;
					newLigne.push_back(move(ptr));
				}
				// Cas du Terrain Non Praticable
				else if (correspondances.getCorrespondanceTNP().find(map_tuiles_code[k]) != correspondances.getCorrespondanceTNP().end()){
					TerrainNonPraticable newTNP(correspondances.getCorrespondanceTNP()[map_tuiles_code[k]],j,i,map_tuiles_code[k]);
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

	 Personnage archer_bleu(ARCHER, true, "Oliver", 21, 1);
	 std::unique_ptr<Personnage> ptrAB(new Personnage(archer_bleu));
	 personnages.push_back(move(ptrAB));
	 
	 Personnage brigand_bleu(BRIGAND, true, "John", 20, 2);
	 std::unique_ptr<Personnage> ptrBB(new Personnage(brigand_bleu));
	 personnages.push_back(move(ptrBB));
	 
	 Personnage chevalier_bleu(CHEVALIER, true, "Bruce", 23, 3);
	 std::unique_ptr<Personnage> ptrCB(new Personnage(chevalier_bleu));
	 personnages.push_back(move(ptrCB));
	 
	 Personnage guerrier_bleu(GUERRIER, true, "Arthur", 22, 4);
	 std::unique_ptr<Personnage> ptrGB(new Personnage(guerrier_bleu));
	 personnages.push_back(move(ptrGB));
	 
	 Personnage archer_rouge(ARCHER, false, "Damian", 1, 21);
	 std::unique_ptr<Personnage> ptrAR(new Personnage(archer_rouge));
	 personnages.push_back(move(ptrAR));
	 
	 Personnage brigand_rouge(BRIGAND, false, "Oswald", 2, 20);
	 std::unique_ptr<Personnage> ptrBR(new Personnage(brigand_rouge));
	 personnages.push_back(move(ptrBR));
	 
	 Personnage chevalier_rouge(CHEVALIER, false, "Jason", 3, 23);
	 std::unique_ptr<Personnage> ptrCR(new Personnage(chevalier_rouge));
	 personnages.push_back(move(ptrCR));
	 
	 Personnage guerrier_rouge(GUERRIER, false, "Slade", 4, 22);
	 std::unique_ptr<Personnage> ptrGR(new Personnage(guerrier_rouge));
	 personnages.push_back(move(ptrGR));	
	
	return 1;
}
