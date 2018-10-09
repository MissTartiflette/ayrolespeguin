#include "state.h"

using namespace state;

Element::Element(){
	this -> nom = "";
	Position position;
	position.setX(0);
	position.setY(0);
	Statistiques statistiques;
	statistiques.setPV(0);
	statistiques.setAttaque(0);
	statistiques.setDefense(0);
	statistiques.setEsquive(0);
	statistiques.setCritique(0);
}

std::string Element::getNom(){
	return nom;
}
