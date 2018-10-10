#include "state.h"

using namespace state;

std::vector<std::vector<Terrain>>& Etat::getGrille(){
	std::vector<std::vector<Terrain>> & refGrille = grille;
	return refGrille;
}

std::vector<Personnage>& Etat::getPersonnages(){
	std::vector<Personnage> & refPersonnages = personnages;
	return refPersonnages;
}

int Etat::getTour(){
	return tour;
}
