#include "state.h"

using namespace state;

TerrainNonPraticable::TerrainNonPraticable(TerrainNonPraticableID id, int newX, int newY, int newCodeTuile):Terrain(newX, newY, newCodeTuile){

	typeID = id;

	if (id == MUR){
	nom = "Mur";
	}
	else if (id == RIVIERE){
	nom = "Riviere";
	}
	else if (id == MONTAGNE){
	nom = "Montagne";
	}
	else if (id == FALAISE){
	nom = "Falaise";
	}
}

bool TerrainNonPraticable::isPraticable(){
	return false;
}

TerrainNonPraticableID TerrainNonPraticable::getTerrainNonPraticableID(){
	return typeID;
}

bool TerrainNonPraticable::equals (Element& other){
	bool resultat;
	if(other.isTerrain()){
		Terrain& other_terrain = static_cast<Terrain&>(other);
		if(!other_terrain.isPraticable()){
			TerrainNonPraticable& other_terrain_non_praticable=static_cast<TerrainNonPraticable&>(other_terrain);
			if(	this->Element::equals(other_terrain_non_praticable) &&
				typeID==other_terrain_non_praticable.getTerrainNonPraticableID()){
				resultat=true;
			}
			else{resultat=false;}
		}
		else{ resultat=false;}
	}
	else{resultat=false;}
	
	return resultat;

}

