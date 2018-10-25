#include "state.h"

using namespace state;

Terrain::Terrain(int newX, int newY, int newCodeTuile):Element(){
	position.setX(newX);
	position.setY(newY);
	codeTuile = newCodeTuile;
}

bool Terrain::isTerrain(){
	return true;
}

bool Terrain::isOccupe(Etat& etat){

	bool resultat = false;
	std::vector<std::unique_ptr<Personnage>> & listePersonnages = etat.getPersonnages();
	size_t i = 0;
	
	/* On compare la position de la case evaluee avec celles des personnages pour savoir si
		une unite est dessus */
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
