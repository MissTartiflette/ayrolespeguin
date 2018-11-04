#include "engine.h"
#include "state.h"

#include <iostream>

using namespace engine;
using namespace state;
using namespace std;

Deplacement::Deplacement(state::Personnage& cible, state::Position& destination):cible(cible), destination(destination){
	
}
    
CommandeID Deplacement::getCommandeID (){
	return DEPLACEMENT;
}

void Deplacement::execute (state::Etat& etat){
	//std::cout<<"position cible: " <<cible.getPosition().getX()<< " " << cible.getPosition().getY()<<std::endl;
	bool b=false;
	
	vector<Position> listePosMouv=cible.getLegalMove(etat);
	//cout<<" taille liste : "<<listePosMouv.size()<<endl;
	for(size_t j=0; j<listePosMouv.size(); j++){
		//std::cout<< "position possible :" << listePosMouv[j].getX() << " "<<listePosMouv[j].getY()<< std::endl;
		if(listePosMouv[j].equals(destination)){
			b=true;
			break;
		}
		
	}
	
	if(b){
		cible.getPosition().setX(destination.getX());
		cible.getPosition().setY(destination.getY());

		cout << cible.getNom() << " s'est déplace sur la case de coordonnees (" << destination.getX() << ", " << destination.getY() << ") avec succes !" << endl;	
	}
	else{
		cerr << "Deplacement non autorise " << endl;
	}
}
