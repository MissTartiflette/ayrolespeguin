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

vector<Position> Personnage::getLegalMove(Etat etat){
	vector<Position> Liste_pos_mouv(2*champMove*(champMove+1)+1);
	for(size_t i=0; i<Liste_pos_mouv.size();i++){
		for(int abscisse=this->position.getX();abscisse<=this->position.getX()+champMove;abscisse++){//parcourt les abscisses
			for(int ordonnee=this->position.getY();ordonnee<=this->position.getY()+champMove;ordonnee++){//parcourt les ordonnées
				if(abscisse+ordonnee<=champMove){
					if(etat.getGrille()[abscisse][ordonnee].isPraticable()==true){
						TerrainPraticable& terprat = dynamic_cast<TerrainPraticable&>(etat.getGrille()[abscisse][ordonnee]);
						if(terprat.isOccupe(etat)==false){
							Liste_pos_mouv[i].setX(this->position.getX()+abscisse);
							Liste_pos_mouv[i].setY(this->position.getY()+ordonnee);
						}
					}
				}
			}
		}
	}
	return Liste_pos_mouv;
//il faut aussi vérifier si le terrain est praticable et s'il n'est pas occupé	
}

vector<Position> Personnage::getLegalAttack(Etat etat){
	vector<Position> Liste_pos_attack(2*champAttack*(champAttack+1)+1);
	for(size_t i=0; i<Liste_pos_attack.size();i++){
		for(int abscisse=this->position.getX();abscisse<=this->position.getX()+champAttack;abscisse++){
			for(int ordonnee=this->position.getY();ordonnee<=this->position.getY()+champAttack;ordonnee++){
				if(abscisse+ordonnee<=champAttack){
					Liste_pos_attack[i].setX(this->position.getX()+abscisse);
					Liste_pos_attack[i].setY(this->position.getY()+ordonnee);
				}
			}
		}
	}
	return Liste_pos_attack;
//il faut aussi vérifier si le terrain est praticable 	

}
