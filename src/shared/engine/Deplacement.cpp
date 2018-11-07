#include "engine.h"
#include "state.h"

#include <iostream>

using namespace engine;
using namespace state;
using namespace std;

Deplacement::Deplacement(state::Personnage& cible, state::Position& destination, bool newJoueur):cible(cible), destination(destination){
	id = DEPLACEMENT;
	joueur = newJoueur;
}

void Deplacement::execute (state::Etat& etat){
	//std::cout<<"position cible: " <<cible.getPosition().getX()<< " " << cible.getPosition().getY()<<std::endl;
	bool b=false;
	if(cible.getChampMove()!=0){
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
			cible.setChampMove(cible.getChampMove()-1);
			cout << cible.getNom() << " s'est déplace sur la case de coordonnees (" << destination.getX() << ", " << destination.getY() << ") avec succes !" << endl;	
			cout << "Il lui reste " << cible.getChampMove() << " points de deplacement." << endl;
		}
		else{
			cerr << "Deplacement non autorise " << endl;
		}
	}
	else {
		cout<< "Deplacement impossible, tous les points de mouvement ont été utilisés pour ce tour." << endl;
	}
}
