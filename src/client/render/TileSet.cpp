#include <string>
#include "render.h"

using namespace render;

TileSet::TileSet(TileSetID newID){
	id=newID;

	if (id == INFOSTILESET){
		cellWidth=0;
		cellHeight=0;
		imageFile="";	
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


