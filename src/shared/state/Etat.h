// Generated by dia2code
#ifndef STATE__ETAT__H
#define STATE__ETAT__H

#include <vector>

namespace state {
  class Personnage;
  class Terrain;
}

#include "Personnage.h"
#include "Terrain.h"

namespace state {

  /// class Etat - 
  class Etat {
    // Associations
    // Attributes
  private:
    std::vector<std::vector<Terrain>> grille;
    std::vector<Personnage> personnages;
    int tour;
    bool fin;
    // Operations
  public:
    std::vector<std::vector<Terrain>>& getGrille ();
    std::vector<Personnage>& getPersonnages ();
    int getTour ();
    // Setters and Getters
  };

};

#endif
