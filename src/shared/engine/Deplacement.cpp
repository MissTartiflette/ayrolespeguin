#include "engine.h"
#include "state.h"
#include <unistd.h>
#include <iostream>

using namespace engine;
using namespace state;
using namespace std;

Deplacement::Deplacement(state::Personnage& cible, state::Position& destination, bool newJoueur):cible(cible), destination(destination){
	id = DEPLACEMENT;
	joueur = newJoueur;
}

void Deplacement::execute (state::Etat& etat){
	string newChaine;
	StateEvent stateEvent(TEXTECHANGED);
	stateEvent.texteInfos = cible.infosToString();
	stateEvent.texteStats = cible.statsToString();
	stateEvent.camp = cible.getCamp();
	int waitTime = 500000;
	
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
				
				stateEvent.stateEventID = ALLCHANGED;
				etat.notifyObservers(stateEvent, etat);
				
				stateEvent.stateEventID = TEXTECHANGED;
				newChaine = cible.getNom() + " se deplace sur la case [" + to_string(destination.getX()) + ", " + to_string(destination.getY()) + "] (encore " + to_string(cible.getChampMove()) + ")";
				cout << newChaine << endl;	
				stateEvent.texte = newChaine;
				etat.notifyObservers(stateEvent, etat);
				stateEvent.stateEventID = PERSONNAGECHANGED;
				stateEvent.texteStats = cible.statsToString();
				etat.notifyObservers(stateEvent, etat);
				stateEvent.stateEventID = TEXTECHANGED;
				//usleep(waitTime);
				
				//cout << cible.getNom() << " se déplace sur la case de coordonnées [" << destination.getX() << ", " << destination.getY() << "] avec succès !" << endl;
				/*
				newChaine = "Il lui reste " + to_string(cible.getChampMove()) + " points de deplacement";
				cout << "\t" << newChaine << endl;	
				stateEvent.texte = newChaine;
				etat.notifyObservers(stateEvent, etat);
				usleep(waitTime);*/
				
				// Nouveau bonus de terrain
				TerrainPraticable& refTerrainDestination = static_cast<TerrainPraticable&>(*etat.getGrille()[destination.getY()][destination.getX()]);
				
								
				if (refTerrainDestination.getTerrainPraticableID() == FORET){
					cible.getStatistiques().setEsquive(cible.getStatistiques().getEsquive()+refTerrainDestination.getStatistiques().getEsquive());
					newChaine = "BONUS de +" + to_string(refTerrainDestination.getStatistiques().getEsquive()) + " en ESQUIVE (Foret)";
					cout << "\t" << newChaine << endl;	
					stateEvent.texte = newChaine;
					etat.notifyObservers(stateEvent, etat);
					stateEvent.texteStats = cible.statsToString();
					stateEvent.stateEventID = PERSONNAGECHANGED;
					etat.notifyObservers(stateEvent, etat);
					stateEvent.stateEventID = TEXTECHANGED;
					usleep(waitTime);					
				}
				else if (refTerrainDestination.getTerrainPraticableID() == COLLINE){
					newChaine = "BONUS de +" + to_string(refTerrainDestination.getStatistiques().getDefense()) + " en DEFENSE (Colline)";
					cout << "\t" << newChaine << endl;	
					stateEvent.texte = newChaine;
					etat.notifyObservers(stateEvent, etat);
					stateEvent.texteStats = cible.statsToString();
					stateEvent.stateEventID = PERSONNAGECHANGED;
					etat.notifyObservers(stateEvent, etat);
					stateEvent.stateEventID = TEXTECHANGED;
					usleep(waitTime);
				}
				
			}
			else{
				newChaine = "Deplacement non autorise";
				cout << newChaine << endl;	
				stateEvent.texte = newChaine;
				etat.notifyObservers(stateEvent, etat);
				stateEvent.stateEventID = PERSONNAGECHANGED;
				etat.notifyObservers(stateEvent, etat);
				stateEvent.stateEventID = TEXTECHANGED;
				usleep(waitTime);
			}
		}
		else {
			newChaine = "Deplacement impossible : aucun point de mouvement";
			cout << newChaine << endl;	
			stateEvent.texte = newChaine;
			etat.notifyObservers(stateEvent, etat);
			stateEvent.stateEventID = PERSONNAGECHANGED;
			etat.notifyObservers(stateEvent, etat);
			stateEvent.stateEventID = TEXTECHANGED;
			usleep(waitTime);
		}
	}
	else if (cible.getStatut()==ATTENTE){
		newChaine = cible.getNom() + " a fini son tour d'actions, il ne peut plus se déplacer.";
		cout << newChaine << endl;	
		stateEvent.texte = newChaine;
		etat.notifyObservers(stateEvent, etat);
		stateEvent.stateEventID = PERSONNAGECHANGED;
		etat.notifyObservers(stateEvent, etat);
		stateEvent.stateEventID = TEXTECHANGED;
		usleep(waitTime); 
	}
	
	cout << "\n" ;
}

Json::Value Deplacement::serialize(){

	Json::Value newCmd;
	newCmd["id"] = id;
	newCmd["joueur"] = (int)joueur;
	newCmd["cible"] = cible.indice;
	newCmd["xDestination"] = destination.getX();
	newCmd["yDestination"] = destination.getY();
	
	return newCmd;
}
