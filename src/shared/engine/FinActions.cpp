#include "engine.h"
#include "state.h"

#include <iostream>

using namespace engine;
using namespace state;
using namespace std;

FinActions::FinActions(state::Personnage& cible, bool newJoueur):cible(cible){
	id = FINACTIONS;
	joueur = newJoueur;
}

void FinActions::execute(state::Etat& etat){
	if (cible.getStatut()!=ATTENTE && cible.getStatut()!=MORT){
		cible.setStatut(ATTENTE);
		cout << cible.getNom() << " termine son tour." << endl;
	}
	else if(cible.getStatut()==ATTENTE){
		cout << cible.getNom() << " est en attente, il ne peut plus effectuer d'actions." <<endl; 
	}
}
