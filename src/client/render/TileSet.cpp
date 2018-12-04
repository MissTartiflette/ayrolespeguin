#include <string>
#include "render.h"

using namespace render;

TileSet::TileSet(TileSetID newID){

	id=newID;

	if (id == INFOSTILESET){
		cellWidth=64;
		cellHeight=64;
		imageFile="res/mugshots_tileset.png";
	}
	
	else if (id==PERSONNAGETILESET){
		cellWidth=16;
		cellHeight=16;
		imageFile="res/personnage_tileset.png";
	}
	
	else if (id==GRILLETILESET){
		cellWidth=16;
		cellHeight=16;
		imageFile="res/grille_tileset.png";
	}
	
	else if (id==CURSEURTILESET){
		cellWidth=16;
		cellHeight=16;
		imageFile="res/curseur_tileset.png";
	}
	
	textureTileset.loadFromFile(imageFile);
}

const int TileSet::getTileSetID(){
	return id;
}

const int TileSet::getCellWidth(){
	return cellWidth;
}

const int TileSet::getCellHeight(){
	return cellHeight;
}

const std::string  TileSet::getImageFile (){
	return imageFile;
}

sf::Texture& TileSet::getTexture(){
	sf::Texture & refTexture = textureTileset;
	return refTexture ;
}




