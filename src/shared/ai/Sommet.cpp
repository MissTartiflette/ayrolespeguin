#include "ai.h"

#include "engine.h"
#include "state.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

using namespace ai;
using namespace engine;
using namespace state;
using namespace std;

Sommet::Sommet(Sommet* predecesseur):predecesseur(predecesseur){
	position.setX(0);
	position.setY(0);
	distanceSource=0;
    distanceDestination=0;
	
}
/**
Sommet::Sommet(const Sommet& rhs){
	
	
	//distanceSource=rhs.getDistanceSource();
	distanceDestination=rhs.getDistanceDestination();
	//Position position;
	position=rhs.getPosition();
    //Sommet* pred;

   	predecesseur=rhs.getPredecesseur();
}
*/
Sommet::Sommet(){
	position.setX(0);
	position.setY(0);
	distanceSource=0;
    distanceDestination=0;
}
/**
Sommet& Sommet::operator=(Sommet& rhs){
	distanceSource=rhs.getDistanceSource();
	distanceDestination=rhs.getDistanceDestination();
	position=rhs.getPosition();
	predecesseur=rhs.getPredecesseur();
}*/

Sommet::~Sommet(){
	//delete predecesseur;
	
}

state::Position& Sommet::getPosition(){
	Position & refPosition = position;
	return refPosition ;
}

void Sommet::setPosition(state::Position newPosition){
	position.setX(newPosition.getX());
	position.setY(newPosition.getY());
}
Sommet* Sommet::getPredecesseur(){
	return predecesseur;
}

const Sommet* Sommet::getPredecesseurC(){
	const Sommet* constpred=predecesseur;
	return constpred;
}

void Sommet::setPredecesseur(Sommet* newPredecesseur){
	predecesseur = newPredecesseur;
}

void Sommet::setPredecesseurC(const Sommet* newPredecesseur){
	Sommet truc=*newPredecesseur;
	predecesseur = &truc;
}

int Sommet::getDistanceSource(){
	return distanceSource;
}

void Sommet::setDistanceSource(int newDistanceSource){
	distanceSource = newDistanceSource;
}

int Sommet::getDistanceDestination(){
	return distanceDestination;
}

void Sommet::setDistanceDestination(int newDistanceDestination){
	distanceDestination = newDistanceDestination;
}

int Sommet::getCoutTotal(){
	return distanceSource + distanceDestination;
}

std::vector<Sommet> Sommet::getVoisins(state::Etat& etat){
	std::vector<Sommet> listeVoisins;
	state::Position positionAjoutee;
	
	
	//On parcourt les abscisses puis les ordonnees de la grille
	for(int abscisse = position.getX() - 1; abscisse <= position.getX() + 1; abscisse++){
		for(int ordonnee = position.getY() - 1; ordonnee <= position.getY() + 1; ordonnee++){
			// On teste les cases autour de la position			
			if(abs(abscisse-position.getX())+abs(ordonnee-position.getY())<=1 && abscisse>=0 && ordonnee>=0 && 			 	abs(abscisse)<etat.getGrille().size() && abs(ordonnee)<etat.getGrille()[abscisse].size()){
				if(etat.getGrille()[abscisse][ordonnee]->isPraticable()){
					if(etat.getGrille()[abscisse][ordonnee]->isOccupe(etat)==-1){
						positionAjoutee.setX(abscisse);
						positionAjoutee.setY(ordonnee);
						if(!positionAjoutee.equals(position)){				
							Sommet newVoisin(this);
							newVoisin.setPosition(positionAjoutee);						
							listeVoisins.push_back(newVoisin);
							listeVoisins.back().setPredecesseur(this);
							listeVoisins.back().setPosition(positionAjoutee);
						}
					}
				}
			}
		}
	}	
	return listeVoisins;	
}
