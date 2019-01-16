#include "engine.h"
#include "state.h"
#include <unistd.h>
#include <iostream>

using namespace engine;
using namespace state;
using namespace std;

FinActions::FinActions(state::Personnage& cible, bool newJoueur):cible(cible){
	id = FINACTIONS;
	joueur = newJoueur;
}

void FinActions::execute(state::Etat& etat){
	string newChaine;
	StateEvent stateEvent(TEXTECHANGED);
	int waitTime = 500000;
	
	if (cible.getStatut()!=ATTENTE && cible.getStatut()!=MORT){
		cible.setStatut(ATTENTE);
		newChaine = cible.getNom() + " termine son tour";
		cout << "\t\t--- " << newChaine << " ---" << endl;
		stateEvent.texte = newChaine;
		etat.notifyObservers(stateEvent, etat);
		usleep(waitTime);
		
		stateEvent.stateEventID = ALLCHANGED;
		etat.notifyObservers(stateEvent, etat);
	}
	else if(cible.getStatut()==ATTENTE){
		newChaine = cible.getNom() + " a deja termine son tour";
		cout << "\t" << newChaine << endl;
		stateEvent.texte = newChaine;
		etat.notifyObservers(stateEvent, etat);
		usleep(waitTime);
		
		stateEvent.stateEventID = ALLCHANGED;
		etat.notifyObservers(stateEvent, etat);
	}
	
	cout << "\n" ;
}

Json::Value FinActions::serialize(){

	Json::Value newCmd;	
	newCmd["id"] = id;
	newCmd["joueur"] = (int)joueur;
	newCmd["cible"] = cible.indice;
	
	return newCmd;
}
