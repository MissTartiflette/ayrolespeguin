// Generated by dia2code
#ifndef ENGINE__ATTAQUE__H
#define ENGINE__ATTAQUE__H


namespace state {
  class Etat;
};
namespace engine {
  class Commande;
}

#include "Commande.h"

namespace engine {

  /// class Attaque - 
  class Attaque : public engine::Commande {
    // Attributes
  private:
    state::Personnage& attaquant;
    state::Personnage& cible;
    bool contreAtk;
    // Operations
  public:
    Attaque (state::Personnage& attaquant, state::Personnage& cible, bool joueur);
    void execute (state::Etat& etat);
    // Setters and Getters
  };

};

#endif
