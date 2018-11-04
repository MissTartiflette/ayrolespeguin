// Generated by dia2code
#ifndef ENGINE__DEPLACEMENT__H
#define ENGINE__DEPLACEMENT__H


namespace state {
  class Etat;
};
namespace engine {
  class Commande;
}

#include "CommandeID.h"
#include "Commande.h"

namespace engine {

  /// class Deplacement - 
  class Deplacement : public engine::Commande {
    // Attributes
  public:
    state::Personnage& cible;
    state::Position& destination;
    // Operations
  public:
    Deplacement (state::Personnage& cible, state::Position& destination);
    CommandeID getCommandeID ();
    void execute (state::Etat& etat);
    // Setters and Getters
  };

};

#endif
