#include "engine.h"
#include "state.h"
#include <unistd.h>
#include <iostream>

using namespace engine;
using namespace state;
using namespace std;



FinActions_Action::FinActions_Action (state::Personnage& cible, bool joueur):cible(cible){
	joueur=joueur;
	oldStatut=cible.getStatut();
}

void FinActions_Action:: apply (state::Etat& etat){
	if (cible.getStatut()!=ATTENTE && cible.getStatut()!=MORT){
		cible.setStatut(ATTENTE);
		cout << "\t\t--- " << cible.getNom() << " termine son tour. ---" << endl;
	}
	else if(cible.getStatut()==ATTENTE){
		cout << cible.getNom() << " a déjà terminé son tour d'actions, il ne peut plus effectuer d'actions !" <<endl; 
	}
	
	cout << "\n" ;
}

void FinActions_Action::undo (state::Etat& etat){
	cible.setStatut(oldStatut);
}
