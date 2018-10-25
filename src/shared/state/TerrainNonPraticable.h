// Generated by dia2code
#ifndef STATE__TERRAINNONPRATICABLE__H
#define STATE__TERRAINNONPRATICABLE__H


namespace state {
  class Element;
  class Terrain;
}

#include "TerrainNonPraticableID.h"
#include "Terrain.h"

namespace state {

  /// class TerrainNonPraticable - 
  class TerrainNonPraticable : public state::Terrain {
    // Associations
    // Attributes
  protected:
    TerrainNonPraticableID typeID;
    // Operations
  public:
    TerrainNonPraticable (TerrainNonPraticableID id, int newX, int newY, int newCodeTuile);
    bool isPraticable ();
    TerrainNonPraticableID getTerrainNonPraticableID ();
    bool equals (Element& other);
    // Setters and Getters
  };

};

#endif
