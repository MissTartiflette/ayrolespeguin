#include "state.h"
#include <iostream>  

using namespace state;
using namespace std;


Personnage::Personnage(TypePersonnageID id, bool newCamp, std::string newNom, int newX, int newY, int newIndice):Element(){

	typeID = id;
	camp=newCamp;
    statut=DISPONIBLE;
    nom = newNom;
    position.setX(newX);
    position.setY(newY);
    indice=newIndice;
    
    if(id == ARCHER){
    	nomArme="Arc";
		codeArme=1;
		champMove=3;
		champAttack=2;
		statistiques.setPV(100);
		statistiques.setAttaque(50);
		statistiques.setDefense(30);
		statistiques.setEsquive(10);
		statistiques.setCritique(30);
		if (newCamp == true){
			codeTuile = 0;
		}
		else{codeTuile = 4;}
    }
    
    else if (id == BRIGAND){
		nomArme="Hache";
		codeArme=2;
		champMove=3;
		champAttack=1;
		statistiques.setPV(100);
		statistiques.setAttaque(50);
		statistiques.setDefense(30);
		statistiques.setEsquive(10);
		statistiques.setCritique(30);
		if (newCamp == true){
			codeTuile = 1;
		}
		else{codeTuile = 5;}
    }
    
    else if (id == CHEVALIER){
		nomArme="Lance";
		codeArme=3;
		champMove=5;
		champAttack=1;
		statistiques.setPV(100);
		statistiques.setAttaque(50);
		statistiques.setDefense(30);
		statistiques.setEsquive(10);
		statistiques.setCritique(30);
		if (newCamp == true){
			codeTuile = 3;
		}
		else{codeTuile = 7;}
    }
    
    else if (id == GUERRIER){
		nomArme="Epee";
		codeArme=4;
		champMove=3;
		champAttack=1;
		statistiques.setPV(100);
		statistiques.setAttaque(50);
		statistiques.setDefense(30);
		statistiques.setEsquive(10);
		statistiques.setCritique(30);
		if (newCamp == true){
			codeTuile = 2;
		}
		else{codeTuile = 6;}
    }
}

bool Personnage::isTerrain (){
	return false;
}

bool Personnage::getCamp (){
	return camp;
}

string Personnage::getNomArme (){
	return nomArme;
}

StatutPersonnageID Personnage::getStatut (){
	return statut;
}

void Personnage::setStatut (StatutPersonnageID newStatut){
	statut=newStatut;
}

vector<Position> Personnage::getLegalMove(Etat& etat){
	// Note : On n'autorise pas le deplacement sur sa propre case car elle est considérée comme occupée
	
	vector<Position> ListePosMouv;
	Position positionAjoutee;
	
	//On parcourt les abscisses puis les ordonnees de la grille
	for(int abscisse = position.getX() - 1; abscisse <=position.getX() + 1; abscisse++){
		for(int ordonnee = position.getY() - 1; ordonnee <= position.getY() + 1; ordonnee++){
		
			// On teste les cases qui sont dans la zone de deplacement du personnage			
			if(abs(abscisse-position.getX())+abs(ordonnee-position.getY())<=1 && abscisse>=0 && ordonnee>=0 && 			 	abs(abscisse)<etat.getGrille().size() && abs(ordonnee)<etat.getGrille()[abscisse].size()){
				if(etat.getGrille()[abscisse][ordonnee]->isPraticable()){
					if(etat.getGrille()[abscisse][ordonnee]->isOccupe(etat)==-1){					
						positionAjoutee.setX(abscisse);
						positionAjoutee.setY(ordonnee);
						ListePosMouv.push_back(positionAjoutee);
						
					}
				}
			}
		}
	}	
	return ListePosMouv;	
}

vector<Position> Personnage::getLegalAttack(Etat& etat){

	vector<Position> ListePosAtq;
	Position positionAjoutee;
	
	//On parcourt les abscisses puis les ordonnees de la grille
	for(int abscisse = this -> position.getX() - champAttack; abscisse <= this -> position.getX()+champAttack; abscisse++){
		for(int ordonnee = this -> position.getY() - champAttack; ordonnee <= this -> position.getY()+champAttack; ordonnee++){
			// On teste les cases qui sont dans la zone d'attaque du personnage
			if(abs(abscisse-position.getX())+abs(ordonnee-position.getY())<=champAttack && abscisse>=0 && ordonnee>=0 && abs(abscisse)<etat.getGrille().size() && abs(ordonnee)<etat.getGrille()[abscisse].size()){
				if(etat.getGrille()[abscisse][ordonnee]->isPraticable()){
					int res =etat.getGrille()[abscisse][ordonnee]->isOccupe(etat);
					if(res!=-1){
						if(this->camp!=etat.getPersonnages()[res]->getCamp()){				
							positionAjoutee.setX(abscisse);
							positionAjoutee.setY(ordonnee);
							// On ne peut attaquer sa propre case
							if(!positionAjoutee.equals(this -> position)){
								ListePosAtq.push_back(positionAjoutee);
							}
						}
					}
				}
			}
		}
	}
	return ListePosAtq;	
}

bool Personnage::equals(Element& other){
	bool resultat;
	if(!other.isTerrain()){
		Personnage& other_personnage = static_cast<Personnage&>(other);

		if(	this->Element::equals(other_personnage) && 
			nomArme==other_personnage.getNomArme() && 
			codeArme==other_personnage.getCodeArme() && 		
			typeID==other_personnage.getType() && 
			champMove==other_personnage.getChampMove() && 
			champAttack==other_personnage.getChampAttack()){
			resultat=true;
		}
		else{resultat=false;}
	}
	else{resultat=false;}	
	return resultat;
}

int Personnage::getChampMove(){
	return champMove;
}

int Personnage::getChampAttack(){
	return champAttack;
}

TypePersonnageID Personnage::getType(){
	return typeID;
}

int Personnage::getCodeArme(){
	return codeArme;
}

void Personnage::setChampMove(int newChampMove){
	champMove = newChampMove;
}

std::string Personnage::infosToString(){
	std::string chaine = nom + "\nPV\nAtk\nDef\nEsq\nCrt";
	return chaine;
}

std::string Personnage::statsToString(){
	std::string chaine = "\n:\t" + to_string(statistiques.getPV()) + "\n:\t" + to_string(statistiques.getAttaque())+"\n:\t" + to_string(statistiques.getDefense()) +"\n:\t" + to_string(statistiques.getEsquive())+"\n:\t"+to_string(statistiques.getCritique());
	return chaine;
}
