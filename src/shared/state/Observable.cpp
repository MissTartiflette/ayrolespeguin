#include "state.h"
#include "engine.h"
using namespace state;
 
void Observable::registerObserver (IObserver* observer){
	observers.push_back(observer);
}

void Observable::notifyObservers (const StateEvent& e, Etat& etat){
	for(auto observer : observers){
		observer->stateChanged(e, etat);
	}
}
