#include <SFML/Graphics.hpp>
#include <string>
#include "render.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>


using namespace render;
using namespace std;
using namespace state;


void Observateur::registerObserver (IObservateur* observer){
	observers.push_back(observer);
}

void Observateur::notifyObservers (Etat& etat, CurseurEventID& touche, int acteur, int cible, state::Position& position, bool rollback){
	
	for(auto observer : observers){
		
		observer->curseurChanged(etat, touche, acteur, cible, position, rollback);
	}
}
