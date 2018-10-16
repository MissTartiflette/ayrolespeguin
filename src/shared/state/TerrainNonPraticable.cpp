#include "state.h"

using namespace state;

TerrainNonPraticable::TerrainNonPraticable(TerrainNonPraticableID id):Terrain(){

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
