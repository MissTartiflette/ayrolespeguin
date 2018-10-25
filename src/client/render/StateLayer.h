// Generated by dia2code
#ifndef RENDER__STATELAYER__H
#define RENDER__STATELAYER__H

#include <vector>

namespace state {
  class Etat;
};
namespace render {
  class TileSet;
  class Surface;
}

#include "state/Etat.h"
#include "TileSet.h"
#include "Surface.h"

namespace render {

  /// class StateLayer - 
  class StateLayer {
    // Associations
    // Attributes
  private:
    state::Etat& etatLayer;
    // Operations
  public:
    StateLayer (state::Etat& etat);
    std::vector<Surface> initSurface (unsigned int longeur, unsigned int largeur);
    // Setters and Getters
  };

};

#endif
