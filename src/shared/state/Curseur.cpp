#include "state.h"

using namespace state;
using namespace std;

Curseur::Curseur (int x, int y){
	nom = "";
	statistiques.setPV(0);
	statistiques.setAttaque(0);
	statistiques.setDefense(0);
	statistiques.setEsquive(0);
	statistiques.setCritique(0);
	position.setX(0);
	position.setY(0);
	codeTuile=0;
}

bool Curseur::isTerrain(){
	return false;
}

void Curseur::move(Position& destination){
	position.setX(destination.getX());
	position.setY(destination.getY());
}
