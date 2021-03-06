// Generated by dia2code
#ifndef ENGINE__OBSERVABLE__H
#define ENGINE__OBSERVABLE__H

#include <vector>

namespace engine {
  class Observer;
}

#include "Observer.h"

namespace engine {

  /// class Observable - 
  class Observable {
    // Associations
    // Attributes
  private:
    mutable std::vector<Observer*> observers;
    // Operations
  public:
    void const registerObserver (Observer* o);
    void const notifyUpdating ();
    void const notifyUpdated ();
    // Setters and Getters
  };

};

#endif
