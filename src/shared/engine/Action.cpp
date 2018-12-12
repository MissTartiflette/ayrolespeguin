#include "engine.h"

using namespace engine;

ActionID Action::getActionID(){
	return id;
}

bool Action::getJoueur(){
	return joueur;
}

void Action:: apply (state::Etat& etat){
	
}

void Action::undo (state::Etat& etat){
	
}
