#include "state.h"

using namespace state;

TerrainPraticable::TerrainPraticable (TerrainPraticableID id, int newX, int newY, int newCodeTuile):Terrain(newX, newY, newCodeTuile){
	
	typeID = id;
	
	if (id == PLAINE){
		nom = "Plaine";
		statistiques.setPV(0);
		statistiques.setAttaque(0);
		statistiques.setDefense(0);
		statistiques.setEsquive(0);
		statistiques.setCritique(0);
	}
	else if (id == FORET){
		nom = "Foret";
		statistiques.setPV(0);
		statistiques.setAttaque(0);
		statistiques.setDefense(0);
		statistiques.setEsquive(20);
		statistiques.setCritique(0);
	}
	else if (id == COLLINE){
		nom = "Colline";
		statistiques.setPV(0);
		statistiques.setAttaque(0);
		statistiques.setDefense(10);
		statistiques.setEsquive(0);
		statistiques.setCritique(0);
	}
	else if (id == PONT){
		nom = "Pont";
		statistiques.setPV(0);
		statistiques.setAttaque(0);
		statistiques.setDefense(0);
		statistiques.setEsquive(0);
		statistiques.setCritique(0);
	}
	else if (id == FORTERESSE){
		nom = "Forteresse";
		statistiques.setPV(5);
		statistiques.setAttaque(0);
		statistiques.setDefense(0);
		statistiques.setEsquive(0);
		statistiques.setCritique(0);
	}
	else if (id == MAISON){
		nom = "Maison";
		statistiques.setPV(1);
		statistiques.setAttaque(0);
		statistiques.setDefense(0);
		statistiques.setEsquive(0);
		statistiques.setCritique(0);
	}
}

bool TerrainPraticable::isPraticable(){
	return true;
}

TerrainPraticableID TerrainPraticable::getTerrainPraticableID(){
	return typeID;
}

bool TerrainPraticable::equals (Element& other){
	bool resultat;
	if(other.isTerrain()){
		Terrain& other_terrain = static_cast<Terrain&>(other);
		if(other_terrain.isPraticable()){
			TerrainPraticable& other_terrain_praticable = static_cast<TerrainPraticable&>(other_terrain);
			if(	this->Element::equals(other_terrain_praticable) &&
				typeID==other_terrain_praticable.getTerrainPraticableID()){
				resultat=true;
			}
			else{resultat=false;}
		}
		else{ resultat=false;}
	}
	else{resultat=false;}
	
	return resultat;

}
