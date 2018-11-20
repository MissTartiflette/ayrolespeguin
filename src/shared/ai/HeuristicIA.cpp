#include "HeuristicIA.h"
#include "engine.h"
#include "state.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

using namespace ai;
using namespace engine;
using namespace state;
using namespace std;


void HeuristicIA::run (engine::Moteur& moteur, sf::RenderWindow& window){
	
	for (size_t i = 0; i < moteur.getEtat().getPersonnages().size(); i++){
		int action=0;
		if (moteur.getEtat().getPersonnages()[i]-> getCamp() == camp){
			while (moteur.getEtat().getPersonnages()[i]->getStatut() != MORT && moteur.getEtat().getPersonnages()[i]->getStatut() != ATTENTE){		
				//int action=0;
				moteur.getEtat().getPersonnages()[i]->setStatut(SELECTIONNE);
				vector<Position> listeAttaques = moteur.getEtat().getPersonnages()[i]-> getLegalAttack(moteur.getEtat());
				if(action!=2){
					if (listeAttaques.size() !=0){
						action=1;
					}
					else{
						action=0;
					}
				}
				
				//cout << "action : " << action << endl;
				// 0 : Cas du deplacement
				if (action==0){
					if(moteur.getEtat().getPersonnages()[i]->getChampMove() != 0){
						int indiceVoisin=this->findIndiceVoisin(moteur, i);
						int choix=this->findchoixOptimalDep(moteur, indiceVoisin, i);
						cout << "choix : " << choix << endl;
						vector<Position> listePos=moteur.getEtat().getPersonnages()[i]->getLegalMove(moteur.getEtat());
						if (choix != -1){
							// Commande de deplacement
							Deplacement deplacement(*moteur.getEtat().getPersonnages()[i], listePos[choix], camp);
							cout << "coordonnées choix : " << listePos[choix].getX() <<" , "<< listePos[choix].getY() << endl;
							unique_ptr<Commande> ptr_deplacement (new Deplacement(deplacement));
							moteur.addCommande(0, move(ptr_deplacement));
							moteur.update(window);
							sleep(1);
						}
					}
					else{
						action=2;
					}
				}
				
				
				// 1 : Cas de l'attaque
				else if (action == 1){
						vector<int> listePV, listeIndice;
						//On parcourt la liste d'attaque composée de toutes les positions d'attaque possibles
						for(size_t j=0; j<listeAttaques.size();j++){
						// On recupere l'indice de chaque cible potentielle dans la liste des personnages
							int indiceCiblePotentielle = moteur.getEtat().getGrille()[listeAttaques[j].getX()][listeAttaques[j].getY()]->isOccupe(moteur.getEtat());
							if (indiceCiblePotentielle != -1){
							//On ajoute dans listePV les PVs de toutes les cibles potentielles
								listePV.push_back(moteur.getEtat().getPersonnages()[indiceCiblePotentielle]->getStatistiques().getPV());
								listeIndice.push_back(indiceCiblePotentielle);
							}
						}
						//On veut attaquer la cible la plus faible									
						int indiceMinPV=indiceMinimum(listePV);//On détermine l'indice du PV min dans listePV  
						int indiceCible=listeIndice[indiceMinPV];//On détermine l'indice de notre cible dans la liste de personnages

						// Commande d'attaque
						Attaque attaque(*moteur.getEtat().getPersonnages()[i], *moteur.getEtat().getPersonnages()[indiceCible], camp);
						unique_ptr<Commande> ptr_attaque (new Attaque(attaque));
						moteur.addCommande(0, move(ptr_attaque));
						moteur.update(window);
						sleep(1);
						
								
				}
				
				// 2 : Cas de fin d'actions
				else if (action == 2){
					FinActions finactions(*moteur.getEtat().getPersonnages()[i], camp);
					unique_ptr<Commande> ptr_finactions (new FinActions(finactions));
					moteur.addCommande(0, move(ptr_finactions));
					moteur.update(window);
					sleep(1);
				}
				
			}		
		}
	}

}

int HeuristicIA::findIndiceVoisin(engine::Moteur& moteur, int personnage){
	
	
	vector<int> listeDistance;
	vector<int> indicePerso;
	
	for(size_t i=0; i<moteur.getEtat().getPersonnages().size(); i++){
		if(moteur.getEtat().getPersonnages()[i]->getCamp()!=moteur.getEtat().getPersonnages()[personnage]->getCamp()){
			listeDistance.push_back(moteur.getEtat().getPersonnages()[personnage]->getPosition().distance(moteur.getEtat().getPersonnages()[i]->getPosition()));
			indicePerso.push_back(i);
		}
	}
	
	int indiceMinDistance=this->indiceMinimum(listeDistance);
	int indiceVoisin= indicePerso[indiceMinDistance];
	return indiceVoisin;
}

int HeuristicIA::findchoixOptimalDep(engine::Moteur& moteur, int indiceVoisin, int personnage){
	int choix=-1;
	vector<int> listeDistanceAVoisin;
	vector<Position> listePosition=moteur.getEtat().getPersonnages()[personnage]->getLegalMove(moteur.getEtat());
	if(listePosition.size()!=0){
		for(size_t i=0; i<listePosition.size(); i++){
			listeDistanceAVoisin.push_back(listePosition[i].distance(moteur.getEtat().getPersonnages()[indiceVoisin]->getPosition()));
		}
		choix=this->indiceMinimum(listeDistanceAVoisin);
	}
	return choix;
}

int HeuristicIA::indiceMinimum(std::vector<int> liste){
	int indiceMin=-1;
	if(liste.size()!=0){//liste non vide
		indiceMin=0;
		int min=liste[0];
		for(size_t i=1; i<liste.size(); i++){
			if(liste[i]<min){
				min=liste[i];
				indiceMin=i;
			}
		}
	}
	return indiceMin;
}
