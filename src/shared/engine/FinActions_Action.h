// Generated by dia2code
#ifndef ENGINE__FINACTIONS_ACTION__H
#define ENGINE__FINACTIONS_ACTION__H


namespace state {
  class Etat;
};
namespace engine {
  class Action;
}

#include "Action.h"

namespace engine {

  /// class FinActions_Action - 
  class FinActions_Action : public engine::Action {
    // Attributes
  private:
    state::Personnage& cible;
    state::StatutPersonnageID oldStatut;
    int oldCodeTuile;
    // Operations
  public:
    FinActions_Action (state::Personnage& cible, bool joueur);
    void apply (state::Etat& etat);
    void undo (state::Etat& etat);
    // Setters and Getters
  };

};

#endif