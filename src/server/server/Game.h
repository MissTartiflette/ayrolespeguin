// Generated by dia2code
#ifndef SERVER__GAME__H
#define SERVER__GAME__H

#include <map>
#include <memory>

namespace server {
  class Player;
}

#include "Player.h"

namespace server {

  /// class Game - 
  class Game {
    // Associations
    // Attributes
  protected:
    int idseq;
    std::map<int, std::unique_ptr<Player>> players;
    // Operations
  public:
    Game ();
    const Player* getPlayer (int id) const;
    int addPlayer (std::unique_ptr<Player> player);
    void setPlayer (int id, std::unique_ptr<Player> player);
    void removePlayer (int id);
    std::map<int, std::unique_ptr<Player>>& getPlayersList ();
    int getIDseq ();
    // Setters and Getters
  };

};

#endif
