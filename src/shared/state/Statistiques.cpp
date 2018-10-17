#include "state.h"

using namespace state;

int Statistiques::getPV(){
    return pv;
}

void Statistiques::setPV (int newPV){
	if (newPV < 0){
		pv = 0;
	}
	else{
    	pv=newPV;
    }
}

int Statistiques::getAttaque(){
    return attaque;
}

void Statistiques::setAttaque(int newAttaque){
	if (newAttaque < 0){
	    attaque=0;
	}
	else{
	    attaque=newAttaque;	
	}
}

int Statistiques::getDefense(){
    return defense;
}

void Statistiques::setDefense(int newDefense){
    if (newDefense < 0){
    	defense=0;
    }
    else{
    	defense = newDefense;
    }
}

int Statistiques::getEsquive(){
    return esquive;
}

void Statistiques::setEsquive(int newEsquive){
	if (newEsquive < 0){
		esquive=0;
	}
	else{
    	esquive=newEsquive;
    }
}

int Statistiques::getCritique(){
    return critique;
}

void Statistiques::setCritique(int newCritique){
	if (newCritique < 0){
		critique=0;
	}
	else{
   		critique=newCritique;
   	}
}

