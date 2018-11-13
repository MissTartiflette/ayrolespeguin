// Generated by dia2code
#ifndef AI__RANDOMIA__H
#define AI__RANDOMIA__H

#include <SFML/Graphics.hpp>

namespace engine {
  class Moteur;
};
namespace sf {
  class RenderWindow;
};
namespace ai {
  class IA;
}

#include "IA.h"

namespace ai {

  /// class RandomIA - 
  class RandomIA : public ai::IA {
    // Operations
  public:
    void run (engine::Moteur& moteur, sf::RenderWindow& window);
    // Setters and Getters
  };

};

#endif