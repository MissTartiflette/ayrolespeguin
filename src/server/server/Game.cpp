#include "server.h"
#include <memory>
#include <map>
#include <vector>
#include <set>

using namespace server;
using namespace std;
using std::unique_ptr;
using std::map;
using std::vector;


Game::Game():idseq(1){   
}

std::map<int, std::unique_ptr<Player>>& Game::getPlayersList(){
	std::map<int, std::unique_ptr<Player>>& ref=players;
	return ref;
}

const Player* Game::getPlayer (int id) const {
    auto ite = players.find(id);
    if (ite == players.cend())
        return nullptr;
    return ite->second.get();
}

int Game::addPlayer (std::unique_ptr<Player> player) {
    int id = idseq++;
    players.insert(std::make_pair(id, std::move(player)));
    return id;
}

void Game::setPlayer (int id, std::unique_ptr<Player> player) {
    players[id] = std::move(player);
    if (id > idseq) {
        idseq = id;
    }
}

void Game::removePlayer (int id) {
    auto ite = players.find(id);
    if (ite == players.end())
        return;
    players.erase(ite);
}

int Game::getIDseq(){
	return idseq;
}
