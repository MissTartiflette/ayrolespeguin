#include <SFML/Graphics.hpp>
#include <string>
#include "render.h"

using namespace render;

PersonnageTileSet::PersonnageTileSet (){
}

const std::string  PersonnageTileSet::getImageFile (){
	return "res/PersonnageTileSet.png";
}

const Tile&  PersonnageTileSet::getTile (state::Element& e){
	int stockage(-1);
	for(unsigned int i=0; i<personnage.size(); i++){
		if(personnage[i].getX()==e.getPosition().getX() && personnage[i].getY()==e.getPosition().getY()){
			
			stockage=i;
			break;
		}
	}
	Tile& refTilePerso=personnage[stockage];
	return refTilePerso;
	
}


