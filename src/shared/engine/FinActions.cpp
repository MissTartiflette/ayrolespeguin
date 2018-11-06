#include "engine.h"
#include "state.h"

#include <iostream>

using namespace engine;
using namespace state;
using namespace std;

FinActions::FinActions(state::Personnage& cible):cible(cible){
	id = FINACTIONS;
}

void FinActions::execute(state::Etat& etat){
	cible.setStatut(ATTENTE);
}
