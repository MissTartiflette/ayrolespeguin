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


void RandomIA::run (engine::Moteur& moteur, sf::RenderWindow& window){
	int randomAction;
	int randomPosition;
	int randomAttaque;
		
	for (unsigned int i = 0; i < moteur.getEtat().getPersonnages().size(); i++ ){
		
		if (moteur.getEtat().getPersonnages()[i]-> getCamp() == camp){
			while (moteur.getEtat().getPersonnages()[i]->getStatut() != MORT && moteur.getEtat().getPersonnages()[i]->getStatut() != ATTENTE){
			
				
				
				moteur.getEtat().getPersonnages()[i]->setStatut(SELECTIONNE);
				srand(time(NULL));
				// Action random entre 3 choix : deplacement, attaque, finActions
				randomAction = rand()%3 ;
								
				// 0 : Cas du deplacement
				if (randomAction == 0 && moteur.getEtat().getPersonnages()[i]->getChampMove() != 0){
					std::vector<Position> listePositions = moteur.getEtat().getPersonnages()[i]-> getLegalMove(moteur.getEtat());
					if (listePositions.size() != 0){
						srand(time(NULL));
						randomPosition = rand()%listePositions.size();
						
						// Commande de deplacement
						Deplacement deplacement(*moteur.getEtat().getPersonnages()[i], listePositions[randomPosition], camp);
						unique_ptr<Commande> ptr_deplacement (new Deplacement(deplacement));
						moteur.addCommande(0, move(ptr_deplacement));
						moteur.update(window);
						sleep(1);
					}
				}
				
				// 1 : Cas de l'attaque
				else if (randomAction == 1){
					std::vector<Position> listeAttaques = moteur.getEtat().getPersonnages()[i]-> getLegalAttack(moteur.getEtat());

					if (listeAttaques.size() !=0){
						srand(time(NULL));
						randomAttaque = rand()%listeAttaques.size();
											
						// On recupere l'indice de la cible dans la liste des personnages
						int indiceCible = moteur.getEtat().getGrille()[listeAttaques[randomAttaque].getX()][listeAttaques[randomAttaque].getY()]->isOccupe(moteur.getEtat());
						if (indiceCible != -1){
							// Commande d'attaque
							Attaque attaque(*moteur.getEtat().getPersonnages()[i], *moteur.getEtat().getPersonnages()[indiceCible], camp);
							unique_ptr<Commande> ptr_attaque (new Attaque(attaque));
							moteur.addCommande(0, move(ptr_attaque));
							moteur.update(window);
							sleep(1);
						}
					}				
				}
				
				// 2 : Cas de fin d'actions
				else if (randomAction == 2){
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
