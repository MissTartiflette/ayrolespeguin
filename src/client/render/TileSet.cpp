#include <SFML/Graphics.hpp>
#include <string>
#include "render.h"

using namespace render;

TileSet::~TileSet(){
}

const Tile&  TileSet::getTile (state::Element& e){}
const Tile&  TileSet::getCharTile (char c){}
