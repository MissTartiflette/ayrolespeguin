// Generated by dia2code
#ifndef RENDER__OBSERVATEUR__H
#define RENDER__OBSERVATEUR__H

#include <vector>

namespace render {
  class IObservateur;
};
namespace state {
  class Etat;
  class Position;
}

#include "state/Position.h"
#include "state/Etat.h"
#include "IObservateur.h"

namespace render {

  /// class Observateur - 
  class Observateur {
    // Associations
    // Attributes
  private:
    std::vector<IObservateur*> observers;
    // Operations
  public:
    void registerObserver (IObservateur* observer);
    void notifyObservers (state::Etat& etat, render::CurseurEventID& touche, int acteur, int cible, state::Position& position, bool rollback);
    // Setters and Getters
  };

};

#endif
