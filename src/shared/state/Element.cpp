#include "state.h"

using namespace state;

Element::Element(){
	nom = "";
	position.setX(0);
	position.setY(0);
	statistiques.setPV(0);
	statistiques.setAttaque(0);
	statistiques.setDefense(0);
	statistiques.setEsquive(0);
	statistiques.setCritique(0);
}

std::string Element::getNom(){
	return nom;
}

Position& Element::getPosition(){
	Position & refPosition = position;
	return refPosition ;
}

Statistiques& Element::getStatistiques(){
	Statistiques & refStatistiques = statistiques;
	return refStatistiques ;
}

bool Element::equals(Element& other){
	bool resultat;
	if(this->position.equals(other.getPosition())==1 && this->nom==other.getNom() && this->statistiques==other.getStatistiques()){
		resultat = true;}
	else{
		resultat = false;
	}
	return resultat;
}

int Element::getCodeTuile (){
	return codeTuile;
}

void Element::setCodeTuile (int newCodeTuile){
	codeTuile = newCodeTuile;
}
