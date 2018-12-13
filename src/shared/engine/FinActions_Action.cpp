#include "engine.h"
#include "state.h"
#include <unistd.h>
#include <iostream>

using namespace engine;
using namespace state;
using namespace std;

FinActions_Action::FinActions_Action (state::Personnage& cible, bool joueur):cible(cible){
	id=FINACTIONS_ACTION;
	joueur=joueur;
	oldStatut=cible.getStatut();
	oldCodeTuile=cible.getCodeTuile();
}

state::Personnage& FinActions_Action::getCible(){
	Personnage& refPersoC=cible;
	return refPersoC;
}

void FinActions_Action:: apply (state::Etat& etat){
	//cout<<"finaction"<<endl;
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
	//cible.setStatut(oldStatut);
	cible.setStatut(state::DISPONIBLE);
	cible.setCodeTuile(oldCodeTuile);
}
