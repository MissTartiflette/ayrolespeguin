#include "engine.h"
#include "render.h"
#include "state.h"
#include <iostream>
#include <unistd.h>

using namespace state;
using namespace engine;
using namespace render;
using namespace std;

Moteur::Moteur () : etatActuel(){	
}

Moteur::~Moteur (){
}

state::Etat& Moteur::getEtat (){
	state::Etat& refEtat=etatActuel;
	return refEtat;
}

void Moteur::addCommandePassive (){
	//commandesActuelles.insert(std::pair<int, std::unique_ptr<Commande>> (commandesActuelles.size(), ptr_cmd));
}

void Moteur::addCommande (int priorite, std::unique_ptr<Commande> ptr_cmd){
	commandesActuelles[priorite]=move(ptr_cmd);
	
}
	
void Moteur::update (sf::RenderWindow& window){
	StateEvent stateEvent(ALLCHANGED);

	map<int, std::unique_ptr<Commande>>::iterator it;

	for(size_t i=0; i<commandesActuelles.size();i++){
		commandesActuelles[i]->execute(etatActuel);
		etatActuel.notifyObservers(stateEvent, etatActuel, window);
		sleep(1);
	}
	for(it=commandesActuelles.begin(); it!=commandesActuelles.end(); it++){
		commandesActuelles.erase(it);
	}
	etatActuel.setTour(etatActuel.getTour()+1);

				
}

 
