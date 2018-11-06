#include "engine.h"
#include "state.h"
#include <iostream> 

using namespace engine;
using namespace state;
using namespace std;

Attaque::Attaque (state::Personnage& attaquant, state::Personnage& cible): attaquant(attaquant), cible(cible){
	id = ATTAQUE;
}
    

void Attaque::execute (state::Etat& etat){
	bool b=false;
	vector<Position> listePosAtq=attaquant.getLegalAttack(etat);

	for(size_t j=0; j<listePosAtq.size(); j++){
		//std::cout<< "position possible pour une attaque :" << listePosAtq[j].getX() << " "<<listePosAtq[j].getY()<< std::endl;
		if(listePosAtq[j].equals(cible.getPosition())){
			b=true;
			break;
		}
		
	}
	if(b){
		if(attaquant.getCamp()!=cible.getCamp()){
			int force_attaquant=attaquant.getStatistiques().getAttaque();
			int pv_cible=cible.getStatistiques().getPV();
			
			cout << attaquant.getNom() << " attaque " << cible.getNom() << "." << endl;
			cible.getStatistiques().setPV(pv_cible-force_attaquant);

			cout << cible.getNom() << " perd " << force_attaquant << " PV.";
			cout << " Il ne lui reste plus que " << cible.getStatistiques().getPV() << " PV."<< endl;
			if(cible.getStatistiques().getPV()==0){
				cible.setStatut(MORT);
				cible.getPosition().setX(-1);
				cible.getPosition().setY(-1);
				cout << cible.getNom() << " est mort." << endl;
			}
			
		}
		
		else{
			cout << attaquant.getNom() << " tente d'attaquer " << cible.getNom() << "." << endl;
			cout << "Mais attendez... " << cible.getNom() << " est votre allie !"; 
			cout << " Heureusement il évite votre " << attaquant.getNomArme() << "." << endl;
		}
	}
	else{
		cout << "Attaque non autorisee !" << endl;
	}
}
