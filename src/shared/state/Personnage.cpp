#include "state.h"
#include <vector>

using namespace state;
using namespace std;


Personnage::Personnage(TypePersonnageID id):Element(){

	typeID = id;
	camp=false;
    nomArme="";
    codeArme=0;
	champMove=0;
	champAttack=0;
    statut=ATTENTE;
    
    if(id == ARCHER){
    	nomArme="Arc";
		codeArme=1;
		champMove=4;
		champAttack=2;
		statistiques.setPV(20);
		statistiques.setAttaque(15);
		statistiques.setDefense(8);
		statistiques.setEsquive(10);
		statistiques.setCritique(15);
    }
    
    else if (id == BRIGAND){
		nomArme="Hache";
		codeArme=2;
		champMove=3;
		champAttack=1;
		statistiques.setPV(20);
		statistiques.setAttaque(15);
		statistiques.setDefense(10);
		statistiques.setEsquive(15);
		statistiques.setCritique(10);
    }
    
    else if (id == CHEVALIER){
		nomArme="Lance";
		codeArme=3;
		champMove=5;
		champAttack=1;
		statistiques.setPV(25);
		statistiques.setAttaque(12);
		statistiques.setDefense(15);
		statistiques.setEsquive(15);
		statistiques.setCritique(10);
    }
    
    else if (id == GUERRIER){
		nomArme="Epee";
		codeArme=4;
		champMove=3;
		champAttack=1;
		statistiques.setPV(20);
		statistiques.setAttaque(15);
		statistiques.setDefense(10);
		statistiques.setEsquive(10);
		statistiques.setCritique(10);
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
	// Note : On autorise le deplacement sur sa propre case
	
	vector<Position> ListePosMouv;
	Position positionAjoutee;
	
	//On parcourt les abscisses puis les ordonnees de la grille
	for(int abscisse = this -> position.getX() - champMove; abscisse <= this -> position.getX()+champMove; abscisse++){
		for(int ordonnee = this -> position.getY() - champMove; ordonnee <= this -> position.getY()+champMove; ordonnee++){
			// On teste les cases qui sont dans la zone de deplacement du personnage
			if(abscisse+ordonnee <= champMove && abscisse+ordonnee >= -champMove){
				if(etat.getGrille()[abscisse][ordonnee]->isPraticable()){
					if(!etat.getGrille()[abscisse][ordonnee]->isOccupe(etat)){						
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
			if(abscisse+ordonnee <= champAttack && abscisse+ordonnee >= -champAttack){
				if(etat.getGrille()[abscisse][ordonnee]->isPraticable()){
					if(etat.getGrille()[abscisse][ordonnee]->isOccupe(etat)){						
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
	return ListePosAtq;	
}
