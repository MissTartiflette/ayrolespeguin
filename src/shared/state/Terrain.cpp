#include "state.h"

using namespace state;

Terrain::Terrain():Element(){
}

bool Terrain::isTerrain(){
	return true;
}

bool Terrain::isOccupe(Etat& etat){
	bool resultat = false;
	std::vector<std::unique_ptr<Personnage>> & listePersonnages = etat.getPersonnages();
	size_t i = 0;
	
	while ( i < listePersonnages.size()){
		if (position.equals(listePersonnages[i]->getPosition())){
			resultat = true;
			break;
		}
		else{
			i++;
		}
	}	
	return resultat; 
}
