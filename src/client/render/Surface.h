// Generated by dia2code
#ifndef RENDER__SURFACE__H
#define RENDER__SURFACE__H

#include <SFML/Graphics.hpp>
#include <string>

namespace sf {
  class Texture;
  class VertexArray;
};
namespace state {
  class Etat;
};
namespace sf {
  class Drawable;
  class Transformable;
}


namespace render {

  /// class Surface - 
  class Surface : public sf::Drawable, public sf::Transformable {
    // Attributes
  private:
    sf::Texture texture;
    sf::VertexArray quads;
    // Operations
  public:
    bool loadGrille (state::Etat& etatLayer, const std::string& tileset, sf::Vector2u tileSize, unsigned int width, unsigned int  height);
    bool loadPersonnage (state::Etat& etatLayer, const std::string& tileset, sf::Vector2u tileSize, unsigned int width, unsigned int height);
    virtual void  draw (sf::RenderTarget& target, sf::RenderStates states) const;
    // Setters and Getters
  };

};

#endif
