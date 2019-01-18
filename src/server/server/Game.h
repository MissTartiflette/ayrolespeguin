// Generated by dia2code
#ifndef SERVER__GAME__H
#define SERVER__GAME__H

#include <thread>
#include <memory>
#include <map>

namespace engine {
  class Moteur;
};
namespace server {
  class Player;
}

#include "engine/Moteur.h"
#include "Player.h"
#include "GameStatus.h"

namespace server {

  /// class Game - 
  class Game {
    // Associations
    // Attributes
  private:
    engine::Moteur moteur;
    std::unique_ptr<std::thread> moteurThread;
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
    engine::Moteur& getMoteur ();
    // Setters and Getters
  };

};

#endif