// Generated by dia2code
#ifndef STATE__TERRAIN__H
#define STATE__TERRAIN__H


namespace state {
  class Element;
}

#include "Element.h"

namespace state {

  /// class Terrain - 
  class Terrain : public state::Element {
    // Operations
  public:
    bool isTerrain ();
    virtual bool isPraticable ();
    // Setters and Getters
  };

};

#endif