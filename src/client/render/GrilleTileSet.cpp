#include <SFML/Graphics.hpp>
#include <string>
#include "render.h"

using namespace render;

GrilleTileSet::GrilleTileSet (){
}

const std::string GrilleTileSet::getImageFile (){
	return "res/tileset.png";
}

const Tile& GrilleTileSet::getTile (state::Element& e){
	int stockage(-1);
	bool t;

	for(unsigned int i=0; i<terrainsPraticables.size(); i++){
		if(terrainsPraticables[i].getX()==e.getPosition().getX() && terrainsPraticables[i].getY()==e.getPosition().getY()){
			
			stockage=i;
			t=true;
			break;
		}
	}
	if(stockage==-1){
		for(unsigned int i=0; i<terrainsNonPraticables.size(); i++){
			if(terrainsNonPraticables[i].getX()==e.getPosition().getX() && terrainsNonPraticables[i].getY()==e.getPosition().getY()){
				
				stockage=i;
				t=false;
				break;
			}
		}
	}
	
	if(t){
		Tile& refTileP=terrainsPraticables[stockage];
		return refTileP;
	}
	else{
		Tile& refTileNP=terrainsNonPraticables[stockage];
		return refTileNP;
	}
}

