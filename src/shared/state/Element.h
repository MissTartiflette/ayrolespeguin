// Generated by dia2code
#ifndef STATE__ELEMENT__H
#define STATE__ELEMENT__H

#include <string>

namespace state {
  class Position;
  class Statistiques;
}

#include "Position.h"
#include "Statistiques.h"

namespace state {

  /// class Element - 
  class Element {
    // Associations
    // Attributes
  protected:
    std::string nom;
    Position position;
    Statistiques statistiques;
    // Operations
  public:
    virtual bool isTerrain () = 0;
    std::string getNom ();
    // Setters and Getters
  };

};

#endif