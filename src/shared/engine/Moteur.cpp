#include "engine.h"
#include "render.h"
#include "state.h"
#include <iostream>
#include <unistd.h>
#include "../../../extern/jsoncpp-1.8.0/jsoncpp.cpp"
#include <SFML/Graphics.hpp>

using namespace state;
using namespace engine;
using namespace render;
using namespace std;

Moteur::Moteur () : etatActuel(){
	changementTour = false;
	joueurActif = true;
	record["tailleReelle"] = 0;
	record["tabCmd"][0] = "";
}

Moteur::~Moteur (){
}

state::Etat& Moteur::getEtat (){
	state::Etat& refEtat=etatActuel;
	return refEtat;
}

void Moteur::addCommande (int priorite, std::unique_ptr<Commande> ptr_cmd){
	
	if (enableRecord){
		Json::Value newCmd = ptr_cmd->serialize();
		record["tabCmd"][record["tailleReelle"].asUInt()] = newCmd;
		record["tailleReelle"] = record["tailleReelle"].asUInt() + 1;
	}
	commandesActuelles[priorite]=move(ptr_cmd);	
}

void Moteur::update (){
	StateEvent stateEvent(ALLCHANGED);

	map<int, std::unique_ptr<Commande>>::iterator it;

	for(size_t i=0; i<commandesActuelles.size();i++){
		// On n'execute que les commandes du joueur dont c'est le tour
		if (commandesActuelles[i]->joueur == joueurActif){
			commandesActuelles[i]->execute(etatActuel);
			//etatActuel.notifyObservers(stateEvent, etatActuel);
			//sleep(1);
		}
	}
	for(it=commandesActuelles.begin(); it!=commandesActuelles.end(); it++){
		commandesActuelles.erase(it);
	}
	commandesActuelles.clear();
}

void Moteur::updateAction (Action* action){
	StateEvent stateEvent(ALLCHANGED);
	
	action->apply(etatActuel); 
	etatActuel.notifyObservers(stateEvent, etatActuel);

}

void Moteur::undo (Action* action){
	StateEvent stateEvent(ALLCHANGED);

	action->undo(etatActuel);
	etatActuel.notifyObservers(stateEvent, etatActuel);
	
}

bool Moteur::verificationFinDeTour(){
	bool tourChange = true;
	bool partieFinie = true;
	
	string newChaine;
	StateEvent stateEvent(TEXTECHANGED);
	int waitTime = 1000000;
	//int countJoueurActif = 0;
	
	for (unsigned int i = 0; i < etatActuel.getPersonnages().size(); i++){
		// Si un personage du joueur actif n'est ni mort ni en attente, son tour n'est pas termine
		if (etatActuel.getPersonnages()[i]->getCamp() == joueurActif){
			//countJoueurActif = countJoueurActif + 1;
			if (etatActuel.getPersonnages()[i]->getStatut() != MORT ){
				if (etatActuel.getPersonnages()[i]->getStatut() != ATTENTE){
					tourChange = false;
				}
			}
			/*
			else{
				countJoueurActif = countJoueurActif - 1;
			}
			*/			
		}
		
		// Si tous les personnages du joueur non actif ne sont pas morts, la partie n'est pas terminee
		else{
			if (etatActuel.getPersonnages()[i]->getStatut() != MORT ){
				partieFinie = false;
				
			}		
		}
	}
	/*
	if (countJoueurActif == 0){
		partieFinie = true;
	}
	*/
	
	if (partieFinie && tourChange){
		newChaine = "Partie Terminee !";
		cout << "\t" << newChaine << endl;
		stateEvent.texte = newChaine;
		etatActuel.notifyObservers(stateEvent, etatActuel);
		usleep(waitTime);
		
		etatActuel.setFin(partieFinie);
		
		//if (countJoueurActif == 0){							 A MODIFIER}
		if (joueurActif){
			newChaine = "L'armee bleue a gagne !";
			cout << "\t" << newChaine << endl;
			stateEvent.texte = newChaine;
			etatActuel.notifyObservers(stateEvent, etatActuel);
			usleep(waitTime);
		}
		else {
			newChaine = "L'armee rouge a gagne !";
			cout << "\t" << newChaine << endl;
			stateEvent.texte = newChaine;
			etatActuel.notifyObservers(stateEvent, etatActuel);
			usleep(waitTime);
		}
		tourChange = false;
	}
		
	else if (tourChange && !partieFinie){
		newChaine = "TOUR TERMINE";
		cout << "\t\t---" << newChaine << " ---\n" << endl;
		stateEvent.texte = newChaine;
		etatActuel.notifyObservers(stateEvent, etatActuel);
		usleep(waitTime);
		
		etatActuel.setTour(etatActuel.getTour()+1);
	}
	
	changementTour = tourChange;
	
	return tourChange;

}

void Moteur::verificationDebutDeTour(){

	string newChaine;
	StateEvent stateEvent(TEXTECHANGED);
	int waitTime = 1000000;
	
	if (changementTour == true){
		// Le maximum de points de vie est de 100
		int nbMaxPV = 100;
	
		joueurActif = !joueurActif;
		
		newChaine = "Changement de joueur";
		cout << "\t-> " << newChaine << " <-" << endl;
		stateEvent.texte = newChaine;
		etatActuel.notifyObservers(stateEvent, etatActuel);
		usleep(waitTime);
		
		newChaine = "TOUR " + to_string(etatActuel.getTour());
		cout << "\t\t--- " << newChaine << " ---\n" << endl;
		stateEvent.texte = newChaine;
		etatActuel.notifyObservers(stateEvent, etatActuel);
		usleep(waitTime);		
		
		for (unsigned int i = 0; i < etatActuel.getPersonnages().size(); i++){
		
			// Personnages du joueur qui termine son tour et qui ne sont pas morts
			if (etatActuel.getPersonnages()[i]->getCamp() != joueurActif && etatActuel.getPersonnages()[i]->getStatut() != MORT){
				// Reinitialisation du statut
				etatActuel.getPersonnages()[i]->setStatut(DISPONIBLE);
				
				//Reinitialisation des points de mouvement
				if (etatActuel.getPersonnages()[i]->getType() != CHEVALIER){
					etatActuel.getPersonnages()[i]->setChampMove(3);
				}
				else {
					etatActuel.getPersonnages()[i]->setChampMove(5);
				}	
			}
			
			// Regain de PV pour les personnages sur des maisons et fortersse en debut de tour
			else if (etatActuel.getPersonnages()[i]->getCamp() == joueurActif && etatActuel.getPersonnages()[i]->getStatut()!=MORT) {
				TerrainPraticable& refTerrainP = static_cast<TerrainPraticable&>(*etatActuel.getGrille()[etatActuel.getPersonnages()[i]->getPosition().getY()][etatActuel.getPersonnages()[i]->getPosition().getX()]);
				
				if(refTerrainP.getTerrainPraticableID() == MAISON || refTerrainP.getTerrainPraticableID() == FORTERESSE){
					if (etatActuel.getPersonnages()[i]->getStatistiques().getPV() + refTerrainP.getStatistiques().getPV()<= nbMaxPV){
						etatActuel.getPersonnages()[i]->getStatistiques().setPV(etatActuel.getPersonnages()[i]->getStatistiques().getPV() + refTerrainP.getStatistiques().getPV());
						// Affichage
						
						newChaine = etatActuel.getPersonnages()[i]->getNom() + " recupere "  + to_string(refTerrainP.getStatistiques().getPV()) + " PV (" + to_string(etatActuel.getPersonnages()[i]->getStatistiques().getPV()) + " PV au total)";
						cout << "+ " << newChaine << " +" << endl;
						stateEvent.texte = newChaine;
						etatActuel.notifyObservers(stateEvent, etatActuel);
						usleep(waitTime);	
						
						/*cout << "+ " << etatActuel.getPersonnages()[i]->getNom() << " récupère " ;
						cout << refTerrainP.getStatistiques().getPV() << " PV.";
						cout << " (" << etatActuel.getPersonnages()[i]->getStatistiques().getPV() << " PV au total). +" << endl;*/
					}
					else if (etatActuel.getPersonnages()[i]->getStatistiques().getPV() == nbMaxPV){
						newChaine = etatActuel.getPersonnages()[i]->getNom() + " a deja atteint son maximum de PV";
						cout << "+ " << newChaine << " +" << endl;
						stateEvent.texte = newChaine;
						etatActuel.notifyObservers(stateEvent, etatActuel);
						usleep(waitTime);
					}
					else {
						
						newChaine = etatActuel.getPersonnages()[i]->getNom() + " recupere "  + to_string(nbMaxPV - etatActuel.getPersonnages()[i]->getStatistiques().getPV()) + " PV (" + to_string(nbMaxPV) + " PV au total)";
						cout << "+ " << newChaine << " +" << endl;
						stateEvent.texte = newChaine;
						etatActuel.notifyObservers(stateEvent, etatActuel);
						usleep(waitTime);
						etatActuel.getPersonnages()[i]->getStatistiques().setPV(nbMaxPV);
					
						/*cout << "+ " << etatActuel.getPersonnages()[i]->getNom() << " récupère " ;
						cout << nbMaxPV -  etatActuel.getPersonnages()[i]->getStatistiques().getPV()<< " PV.";
						etatActuel.getPersonnages()[i]->getStatistiques().setPV(nbMaxPV);
						cout << " (" << etatActuel.getPersonnages()[i]->getStatistiques().getPV() << " PV au total). +" << endl;*/
					}
				}
			}
		}
		changementTour = !changementTour;
	}
}

bool Moteur::getJoueurActif(){
	return joueurActif;
}

void Moteur::addAction(Action* newAction){
	listeActionsJouees.push_back(move(newAction));
}

void Moteur::curseurChanged(Etat& etat, CurseurEventID& touche, int acteur, int cible, state::Position& position, bool rollback){
	
	if(touche==A){
		if (rollback){
			Attaque_Action attaque(*etatActuel.getPersonnages()[acteur], *etatActuel.getPersonnages()[cible], etatActuel.getPersonnages()[acteur]->getCamp());
			Attaque_Action* ptr_attaque (new Attaque_Action(attaque));
			addAction(move(ptr_attaque));
			updateAction(move(ptr_attaque));
		}
		else{
			Attaque attaque(*etatActuel.getPersonnages()[acteur], *etatActuel.getPersonnages()[cible], etatActuel.getPersonnages()[acteur]->getCamp());
			unique_ptr<Commande> ptr_attaque (new Attaque(attaque));
			addCommande(0, move(ptr_attaque));
		}
		
	}
	else if(touche==Z){
		if (rollback){
			FinActions_Action finaction(*etatActuel.getPersonnages()[acteur], etatActuel.getPersonnages()[acteur]->getCamp());
			FinActions_Action* ptr_finaction (new FinActions_Action(finaction));
			addAction(move(ptr_finaction));
			updateAction(move(ptr_finaction));
		}
		else{
			FinActions finaction(*etatActuel.getPersonnages()[acteur], etatActuel.getPersonnages()[acteur]->getCamp());
			unique_ptr<Commande> ptr_finaction (new FinActions(finaction));
			addCommande(0, move(ptr_finaction));
		}
	}
	else if(touche==M){
		if (rollback){
			Dep_Action deplacement(*etatActuel.getPersonnages()[acteur], position, joueurActif);
			Dep_Action* ptr_deplacement (new Dep_Action(deplacement));
			addAction(move(ptr_deplacement));
			updateAction(move(ptr_deplacement));
		}
		else{
			Deplacement deplacement(*etatActuel.getPersonnages()[acteur], position, joueurActif);
			unique_ptr<Commande> ptr_deplacement (new Deplacement(deplacement));
			addCommande(0, move(ptr_deplacement));
		}
	}
	else if(touche==UPDATE){
		update();
	}

	else if(touche==R){
		if (rollback){
			cout << listeActionsJouees.size() << " actions a annuler" << endl;
			if (listeActionsJouees.size()>0){
				for(int i = listeActionsJouees.size()-1; i >= 0; i--){
					undo(move(listeActionsJouees[i]));
					
					cout << "Annulation " << i << " effectuee" << endl;
					usleep(750000);
					listeActionsJouees.pop_back();
				}
			}
		}
	}
}

void Moteur::setEnableRecord(bool val){
	enableRecord = val;
}

Json::Value Moteur::getRecord(){
	return record;
}

