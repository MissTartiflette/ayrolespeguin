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
	cible.setStatut(ATTENTE);
	cout << cible.getNom() << " termine son tour." << endl;
}
