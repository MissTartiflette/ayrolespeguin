// Generated by dia2code
#ifndef AI__IA__H
#define AI__IA__H

#include <vector>
#include <memory>

namespace engine {
  class Moteur;
  class Commande;
};
namespace state {
  class Etat;
}

#include "engine/Commande.h"

namespace ai {

  /// class IA - 
  class IA {
    // Attributes
  protected:
    bool camp     = false;
    // Operations
  public:
    virtual void run (engine::Moteur& moteur) = 0;
    void setCamp (bool newCamp);
    bool getCamp ();
  protected:
    void listeCommandes (std::vector<std::unique_ptr<engine::Commande>>& liste, state::Etat& etat);
    // Setters and Getters
  };

};

#endif
