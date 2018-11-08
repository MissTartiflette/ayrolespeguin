#include "engine.h"
#include "state.h"

#include <iostream>

using namespace engine;
using namespace state;
using namespace std;

Deplacement::Deplacement(state::Personnage& cible, state::Position& destination, bool newJoueur):cible(cible), destination(destination){
	id = DEPLACEMENT;
	joueur = newJoueur;
}

void Deplacement::execute (state::Etat& etat){

	bool deplacementPossible=false;
	if (cible.getStatut()!=ATTENTE && cible.getStatut()!=MORT){

		if(cible.getChampMove()!=0){
			vector<Position> listePosMouv=cible.getLegalMove(etat);
			
			for(size_t j=0; j<listePosMouv.size(); j++){			
				if(listePosMouv[j].equals(destination)){
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
				cible.getPosition().setX(destination.getX());
				cible.getPosition().setY(destination.getY());
				cible.setChampMove(cible.getChampMove()-1);
				cout << cible.getNom() << " se déplace sur la case de coordonnées [" << destination.getX() << ", " << destination.getY() << "] avec succès !" << endl;	
				cout << "\tIl lui reste " << cible.getChampMove() << " points de deplacement." << endl;
				
				// Nouveau bonus de terrain
				TerrainPraticable& refTerrainDestination = static_cast<TerrainPraticable&>(*etat.getGrille()[destination.getY()][destination.getX()]);
				
								
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
			cout<< "Deplacement impossible, tous les points de mouvement ont été utilisés pour ce tour." << endl;
		}
	}
	else if (cible.getStatut()==ATTENTE){
		cout << cible.getNom() << " a terminé toutes son tour d'actions, il ne peut plus se déplacer." <<endl; 
	}
	
	cout << "\n" ;
}
