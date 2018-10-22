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

bool TerrainNonPraticable::equals (Element& other){
	bool resultat;
	if(other.isTerrain()){
		Terrain& t = static_cast<Terrain&>(other);
		if(!t.isPraticable()){
			TerrainNonPraticable& tNP = static_cast<TerrainNonPraticable&>(t);
			if(this->Element::equals(tNP) && typeID==tNP.getTerrainNonPraticableID()){
				resultat=true;
			}
			else{resultat=false;}
		}
		else{ resultat=false;}
	}
	else{resultat=false;}
	
	return resultat;

}

