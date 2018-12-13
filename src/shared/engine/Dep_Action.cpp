#include "engine.h"
#include "state.h"

#include <iostream>

using namespace engine;
using namespace state;
using namespace std;

Dep_Action::Dep_Action (state::Personnage& cible, state::Position& destination, bool joueur):cible(cible){
	id=DEP_ACTION;
	newPos=destination;
	oldPos=cible.getPosition();
	oldStat=cible.getStatistiques();
	oldChampMove=cible.getChampMove();
	this->joueur = joueur;
}

state::Position& Dep_Action::getDestination(){
	Position& refPos=newPos;
	return refPos;
}

state::Personnage& Dep_Action::getCible(){
	Personnage& refPerso=cible;
	return refPerso;
}

void Dep_Action::apply (state::Etat& etat){
	//cout<<"depAction"<<endl;
	bool deplacementPossible=false;
	if (cible.getStatut()!=ATTENTE && cible.getStatut()!=MORT){

		if(cible.getChampMove()!=0){
			vector<Position> listePosMouv=cible.getLegalMove(etat);
			
			for(size_t j=0; j<listePosMouv.size(); j++){			
				if(listePosMouv[j].equals(newPos)){
					deplacementPossible=true;
					break;
				}			
			}
			
			if(deplacementPossible){
				
				// Deduction du bonus precedent
				TerrainPraticable& refTerrainActuel = static_cast<TerrainPraticable&>(*etat.getGrille()[cible.getPosition().getY()][cible.getPosition().getX()]);
				
				cible.getStatistiques().setAttaque(cible.getStatistiques().getAttaque()-refTerrainActuel.getStatistiques().getAttaque());
				cible.getStatistiques().setDefense(cible.getStatistiques().getDefense()-refTerrainActuel.getStatistiques().getDefense());
				cible.getStatistiques().setEsquive(cible.getStatistiques().getEsquive()-refTerrainActuel.getStatistiques().getEsquive());
				cible.getStatistiques().setCritique(cible.getStatistiques().getCritique()-refTerrainActuel.getStatistiques().getCritique());
				
				// Modification de Position
				cible.getPosition().setX(newPos.getX());
				cible.getPosition().setY(newPos.getY());
				cible.setChampMove(cible.getChampMove()-1);
				cout << cible.getNom() << " se déplace sur la case de coordonnées [" << newPos.getX() << ", " << newPos.getY() << "] avec succès !" << endl;	
				cout << "\tIl lui reste " << cible.getChampMove() << " points de deplacement." << endl;
				
				// Nouveau bonus de terrain
				TerrainPraticable& refTerrainDestination = static_cast<TerrainPraticable&>(*etat.getGrille()[newPos.getY()][newPos.getX()]);
				
								
				if (refTerrainDestination.getTerrainPraticableID() == FORET){
					cible.getStatistiques().setEsquive(cible.getStatistiques().getEsquive()+refTerrainDestination.getStatistiques().getEsquive());
					cout << "+ Il obtient un bonus de +" ;
					cout << refTerrainDestination.getStatistiques().getEsquive() << " en ESQUIVE sur cette case FORET. +" << endl;
					
				}
				else if (refTerrainDestination.getTerrainPraticableID() == COLLINE){
					cible.getStatistiques().setDefense(cible.getStatistiques().getDefense()+refTerrainDestination.getStatistiques().getDefense());
					cout << "+ Il obtient un bonus de +" ;
					cout << refTerrainDestination.getStatistiques().getDefense() << " en DEFENSE sur cette case COLLINE. +" << endl;
				}
				
			}
			else{
				cerr << "Deplacement non autorise " << endl;
			}
		}
		else {
			cout<< "Deplacement impossible, tous les points de " << cible.getNom() << " de mouvement ont été utilisés pour ce tour." << endl;
		}
	}
	else if (cible.getStatut()==ATTENTE){
		cout << cible.getNom() << " a terminé toutes son tour d'actions, il ne peut plus se déplacer." <<endl; 
	}
	
	cout << "\n" ;
}

void Dep_Action::undo (state::Etat& etat){
	cible.setStatut(state::DISPONIBLE);
	cible.getPosition().setX(oldPos.getX());
	cible.getPosition().setY(oldPos.getY());

	cible.getStatistiques().setPV(oldStat.getPV());
	cible.getStatistiques().setAttaque(oldStat.getAttaque());
	cible.getStatistiques().setDefense(oldStat.getDefense());
	cible.getStatistiques().setCritique(oldStat.getCritique());
	cible.getStatistiques().setEsquive(oldStat.getEsquive());

	cible.setChampMove(oldChampMove);
}
