#include "server.h"

using namespace server;
using namespace std;


//Game& game;
  
GameService::GameService (Game& game):AbstractService("/game"), game(game){
}

HttpStatus GameService::get (Json::Value& out, int id) const{
	const Player* player = game.getPlayer(id);
    if (!player)
        throw ServiceException(HttpStatus::NOT_FOUND,"Invalid player id");
    out["name"] = player->name;
    out["free"] = player->free;
    return HttpStatus::OK;
}

bool GameService::isVersion(){
	return false;
}
