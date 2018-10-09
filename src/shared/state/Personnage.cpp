#include "state.h"
using namespace state;
using namespace std;


Personnage::Personnage(){
	camp=false;
    nomArme="";
    codeArme=0;
    statut=ATTENTE;
}

bool Personnage::isTerrain (){
	return false;
}

bool Personnage::getCamp (){
	return camp;
}

string Personnage::getNomArme (){
	return nomArme;
}

StatutPersonnageID Personnage::getStatut (){
	return statut;
}

void Personnage::setStatut (StatutPersonnageID newStatut){
	statut=newStatut;
}
