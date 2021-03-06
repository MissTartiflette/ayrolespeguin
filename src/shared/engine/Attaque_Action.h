// Generated by dia2code
#ifndef ENGINE__ATTAQUE_ACTION__H
#define ENGINE__ATTAQUE_ACTION__H


namespace state {
  class Etat;
};
namespace engine {
  class Action;
}

#include "Action.h"

namespace engine {

  /// class Attaque_Action - 
  class Attaque_Action : public engine::Action {
    // Attributes
  private:
    state::Personnage& attaquant;
    state::Personnage& cible;
    bool contreAtk;
    state::Statistiques oldStatistiquesAttaquant;
    state::Statistiques oldStatistiquesCible;
    state::Position oldPositionCible;
    state::Position oldPositionAttaquant;
    state::StatutPersonnageID oldStatutAttaquant;
    state::StatutPersonnageID oldStatutCible;
    // Operations
  public:
    Attaque_Action (state::Personnage& attaquant, state::Personnage& cible, bool joueur);
    void apply (state::Etat& etat);
    void undo (state::Etat& etat);
    state::Personnage& getCible ();
    state::Personnage& getAttaquant ();
    // Setters and Getters
  };

};

#endif
