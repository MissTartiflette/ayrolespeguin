// Generated by dia2code
#ifndef RENDER__STATELAYER__H
#define RENDER__STATELAYER__H

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include <string>

namespace sf {
  class Font;
};
namespace state {
  class Etat;
};
namespace sf {
  class RenderWindow;
};
namespace state {
  class IObserver;
};
namespace render {
  class TileSet;
  class Surface;
}

#include "state/IObserver.h"
#include "state/Etat.h"
#include "TileSet.h"
#include "Surface.h"

namespace render {

  /// class StateLayer - 
  class StateLayer : public state::IObserver {
    // Associations
    // Attributes
  protected:
    std::vector<std::unique_ptr<TileSet>> tilesets;
    std::vector<std::unique_ptr<Surface>> surfaces;
    sf::Font police;
    // Operations
  public:
    StateLayer (state::Etat& etat);
    std::vector<std::unique_ptr<TileSet>>& getTilesets ();
    std::vector<std::unique_ptr<Surface>>& getSurfaces ();
    void initSurfaces (state::Etat& etat);
    void stateChanged (const state::StateEvent& e, state::Etat& etat, sf::RenderWindow& window);
    void draw (sf::RenderWindow& window);
    void writeTexteAction (const std::string chaine, sf::RenderWindow& window);
    // Setters and Getters
  };

};

#endif
