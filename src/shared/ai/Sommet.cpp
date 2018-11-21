#include "ai.h"

using namespace ai;

Sommet::Sommet(state::Position newPosition){
	position = newPosition;
}

state::Position Sommet::getPosition(){
	return position;
}

Sommet* Sommet::getPredecesseur(){
	return predecesseur;
}

void Sommet::setPredecesseur(Sommet* newPredecesseur){
	predecesseur = newPredecesseur;
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
						Sommet newVoisin(positionAjoutee);						
						listeVoisins.push_back(newVoisin);
					}
				}
			}
		}
	}	
	return listeVoisins;	
}
