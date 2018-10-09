#include "state.h"

using namespace state;

int Statistiques::getPV(){
    return pv;
}

void Statistiques::setPV (int newPV){
    pv=newPV;
}

int Statistiques::getAttaque(){
    return attaque;
}

void Statistiques::setAttaque(int newAttaque){
    attaque=newAttaque;
}

int Statistiques::getDefense(){
    return defense;
}

void Statistiques::setDefense(int newDefense){
    defense=newDefense;
}

int Statistiques::getEsquive(){
    return esquive;
}

void Statistiques::setEsquive(int newEsquive){
    esquive=newEsquive;
}

int Statistiques::getCritique(){
    return critique;
}

void Statistiques::setCritique(int newCritique){
   critique=newCritique;
}


