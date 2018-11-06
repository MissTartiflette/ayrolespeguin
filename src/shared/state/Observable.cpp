#include "state.h"
#include "engine.h"
#include <iostream>

using namespace state;
using namespace std;

void Observable::registerObserver (IObserver* observer){
	observers.push_back(observer);
}

void Observable::notifyObservers (const StateEvent& e, Etat& etat, sf::RenderWindow& window){
	
	for(auto observer : observers){
		observer->stateChanged(e, etat, window);
	}
}
