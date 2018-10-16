#include "state.h"

using namespace state;

std::vector<std::vector<std::unique_ptr<Terrain>>>& Etat::getGrille(){
	std::vector<std::vector<std::unique_ptr<Terrain>>> & refGrille = grille;
	return refGrille;
}

std::vector<std::unique_ptr<Personnage>>& Etat::getPersonnages(){
	std::vector<std::unique_ptr<Personnage>> & refPersonnages = personnages;
	return refPersonnages;
}

int Etat::getTour(){
	return tour;
}
