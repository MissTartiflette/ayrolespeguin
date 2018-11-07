#include "state.h"
#include <iostream>
using namespace state;
using namespace std;

Terrain::Terrain(int newX, int newY, int newCodeTuile):Element(){
	position.setX(newX);
	position.setY(newY);
	codeTuile = newCodeTuile;
}

bool Terrain::isTerrain(){
	return true;
}

int Terrain::isOccupe(Etat& etat){
	int resultat=-1;
	vector<std::unique_ptr<Personnage>> & listePersonnages = etat.getPersonnages();
	
	/* On compare la position de la case evaluee avec celles des personnages pour savoir si
		une unite est dessus */
	for(size_t i=0; i < listePersonnages.size(); i++){
		if (position.equals(listePersonnages[i]->getPosition())){
			resultat = i;
			break;
		
		}
		
	}	
	
	return resultat;
	
}
