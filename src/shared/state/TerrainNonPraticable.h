// Generated by dia2code
#ifndef STATE__TERRAINNONPRATICABLE__H
#define STATE__TERRAINNONPRATICABLE__H


namespace state {
  class Terrain;
}

#include "TerrainNonPraticableID.h"
#include "Terrain.h"

namespace state {

  /// class TerrainNonPraticable - 
  class TerrainNonPraticable : public state::Terrain {
    // Associations
    // Operations
  public:
    TerrainNonPraticable (TerrainNonPraticableID id);
    bool isPraticable ();
    // Setters and Getters
  };

};

#endif
