// Generated by dia2code
#ifndef STATE__POSITION__H
#define STATE__POSITION__H

#include <vector>

namespace state {
  class Position;
  class Etat;
}

namespace state {

  /// class Position - 
  class Position {
    // Attributes
  private:
    int x;
    int y;
    // Operations
  public:
    Position ();
    Position (int x, int y);
    int getX ();
    int getY ();
    void setX (int newX);
    void setY (int newY);
    bool equals (Position& other);
    int distance (Position& other);
    std::vector<Position> getVoisins (Etat& etat);
    // Setters and Getters
  };

};

#endif
