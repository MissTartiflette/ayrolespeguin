// Generated by dia2code
#ifndef STATE__POSITION__H
#define STATE__POSITION__H


namespace state {
  class Position;
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
    int getX ();
    int getY ();
    void setX (int newX);
    void setY (int newY);
    bool equals (Position& other);
    // Setters and Getters
  };

};

#endif
