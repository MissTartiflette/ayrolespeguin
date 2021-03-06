// Generated by dia2code
#ifndef ENGINE__DEP_ACTION__H
#define ENGINE__DEP_ACTION__H


namespace state {
  class Etat;
};
namespace engine {
  class Action;
}

#include "Action.h"

namespace engine {

  /// class Dep_Action - 
  class Dep_Action : public engine::Action {
    // Attributes
  private:
    state::Position oldPos;
    state::Position newPos;
    state::Personnage& cible;
    state::Statistiques oldStat;
    int oldChampMove;
    // Operations
  public:
    Dep_Action (state::Personnage& cible, state::Position& destination, bool joueur);
    void apply (state::Etat& etat);
    void undo (state::Etat& etat);
    state::Personnage& getCible ();
    state::Position& getDestination ();
    // Setters and Getters
  };

};

#endif
