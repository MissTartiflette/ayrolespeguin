// Generated by dia2code
#ifndef STATE__IOBSERVER__H
#define STATE__IOBSERVER__H


namespace state {
  class StateEvent;
  class Etat;
}

#include "StateEvent.h"

namespace state {

  // interface
  /// class IObserver - 
  class IObserver {
    // Operations
  public:
    virtual void stateChanged (const StateEvent& e, Etat& etat) = 0;
    // Setters and Getters
  };

};

#endif
