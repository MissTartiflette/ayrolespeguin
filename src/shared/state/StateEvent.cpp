#include "state.h"

using namespace state;
 
StateEvent::StateEvent (StateEventID se):stateEventID(se){
	
}

void StateEvent::setStateEventID(StateEventID newID){
	stateEventID=newID;
}
