#include "state.h"

using namespace state;

TerrainPraticable::TerrainPraticable (TerrainPraticableID id){
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
	else {
		nom = "";
		statistiques.setPV(0);
		statistiques.setAttaque(0);
		statistiques.setDefense(0);
		statistiques.setEsquive(0);
		statistiques.setCritique(0);
	}
}

bool TerrainPraticable::isPraticable(){
	return true;
}

bool TerrainPraticable::isOccupe(Etat etat){
	bool resultat = false;
	std::vector<Personnage> & listePersonnages = etat.getPersonnages();
	size_t i = 0;
	
	while ( i < listePersonnages.size()){
		if (position.equals(listePersonnages[i].getPosition())){
			resultat = true;
			break;
		}
		else{
			i++;
		}
	}	
	return resultat; 
}

TerrainPraticableID TerrainPraticable::getTerrainPraticableID(){
	return id;
}
