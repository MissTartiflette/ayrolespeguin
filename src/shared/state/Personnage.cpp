#include "state.h"
#include <vector>
using namespace state;
using namespace std;


Personnage::Personnage(){
	camp=false;
    nomArme="";
    codeArme=0;
	champMove=0;
	champAttack=0;
    statut=ATTENTE;
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
	/* Note : On autorise le déplacement sur sa propre case
	 */
	vector<Position> ListePosMouv;
	Position positionAjoutee;
	
	//On parcourt les abscisses de la grille
	for(int abscisse = this -> position.getX() - champMove; abscisse <= this -> position.getX()+champMove; abscisse++){
		//On parcourt les ordonnées de la grille
		for(int ordonnee = this -> position.getY() - champMove; ordonnee <= this -> position.getY()+champMove; ordonnee++){
			// On teste les cases qui sont dans la zone de deplacement du personnage
			if(abscisse+ordonnee <= champMove && abscisse+ordonnee >= -champMove){
				if(etat.getGrille()[abscisse][ordonnee].isPraticable()){
					if(!etat.getGrille()[abscisse][ordonnee].isOccupe(etat)){						
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
	
	//On parcourt les abscisses de la grille
	for(int abscisse = this -> position.getX() - champAttack; abscisse <= this -> position.getX()+champAttack; abscisse++){
		//On parcourt les ordonnées de la grille
		for(int ordonnee = this -> position.getY() - champAttack; ordonnee <= this -> position.getY()+champAttack; ordonnee++){
			// On teste les cases qui sont dans la zone de deplacement du personnage
			if(abscisse+ordonnee <= champAttack && abscisse+ordonnee >= -champAttack){
				if(etat.getGrille()[abscisse][ordonnee].isPraticable()){
					if(etat.getGrille()[abscisse][ordonnee].isOccupe(etat)){						
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
