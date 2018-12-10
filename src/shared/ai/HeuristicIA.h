// Generated by dia2code
#ifndef AI__HEURISTICIA__H
#define AI__HEURISTICIA__H

#include <SFML/Graphics.hpp>
#include <vector>

namespace engine {
  class Moteur;
};
namespace sf {
  class RenderWindow;
};
namespace state {
  class Position;
};
namespace ai {
  class Sommet;
  class IA;
}

#include "state/Position.h"
#include "Sommet.h"
#include "IA.h"

namespace ai {

  /// class HeuristicIA - 
  class HeuristicIA : public ai::IA {
    // Associations
    // Operations
  public:
    void run (engine::Moteur& moteur, sf::RenderWindow& window);
    std::vector<state::Position> AlgorithmeA (engine::Moteur& moteur, int i, state::Position posDestination);
    int isPresentSom (std::vector<Sommet> listeSommet, Sommet sommet);
    int isPresentPos (std::vector<Sommet> listeSommet, state::Position position);
    int findIndiceVoisin (engine::Moteur& moteur, int personnage);
    int indiceMinimum (std::vector<int>  liste);
    int indiceMaximum (std::vector<int> liste);
    std::vector<state::Position> findRefuge (engine::Moteur& moteur, int personnage);
    std ::vector<state::Position> findObjectif (engine::Moteur& moteur, int indicePerso);
    // Setters and Getters
  };

};

#endif
