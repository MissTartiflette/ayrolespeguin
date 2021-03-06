// Generated by dia2code
#ifndef STATE__CURSEUR__H
#define STATE__CURSEUR__H


namespace state {
  class Position;
  class Element;
}

#include "Element.h"

namespace state {

  /// class Curseur - 
  class Curseur : public state::Element {
    // Operations
  public:
    Curseur (int x = 0, int y = 0);
    bool isTerrain ();
    void move (Position& destination);
    // Setters and Getters
  };

};

#endif
