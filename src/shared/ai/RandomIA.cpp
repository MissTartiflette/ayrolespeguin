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
	// L'IA effectue ces actions uniquement si c'est son tour
	if(moteur.getJoueurActif()==false){
		int randomAction;
		int randomPosition;
		int randomAttaque;
		bool premierEssai = true;
		bool deplacementImpossible = false;
		bool attaqueImpossible = false;
			
		for (unsigned int i = 0; i < moteur.getEtat().getPersonnages().size(); i++ ){
			// Reinitialisations de changement de personnage
			deplacementImpossible = false;
			attaqueImpossible = false;
			premierEssai = true;
			
			// Controle des personnages de l'IA (camp = false)
			if (moteur.getEtat().getPersonnages()[i]-> getCamp() == camp){
				while (moteur.getEtat().getPersonnages()[i]->getStatut() != MORT && moteur.getEtat().getPersonnages()[i]->getStatut() != ATTENTE){
					if(premierEssai == true){
						cout<< "\t\t-> Début du tour de " << moteur.getEtat().getPersonnages()[i]->getNom() << " <-" << endl;
						premierEssai = false;
					}
					
					moteur.getEtat().getPersonnages()[i]->setStatut(SELECTIONNE);
										
					/* 	Action random entre 3 choix : deplacement (0), attaque(1), finActions(2)
						Si le personnage vient de tenter une action et qu'elle fut impossible, 
						on élimine ce choix pour qu'il ne la retente pas tant que cela reste impossible.
					*/
					srand(time(NULL));
					if (deplacementImpossible == true && attaqueImpossible == true){
						randomAction = 2;
					}
					else if (deplacementImpossible == true && attaqueImpossible == false){
						randomAction = rand()%2+1 ;
					}
					else if (deplacementImpossible == false && attaqueImpossible == true){
						randomAction = rand()%2;
						if(randomAction == 1){
							randomAction = 2;
						}
					}
					else{randomAction = rand()%3;}
														
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
							attaqueImpossible = false;
							sleep(1);
						}
						else{deplacementImpossible = true;}
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
								deplacementImpossible = false;
								sleep(1);
							}
						}
						else{attaqueImpossible = true;}				
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
}
