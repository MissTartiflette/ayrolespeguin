#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <map>
#include <memory>
#include <unistd.h>

// Les lignes suivantes ne servent qu'à vérifier que la compilation avec SFML fonctionne
#include <SFML/Graphics.hpp>


void testSFML() {
    sf::Texture texture;
}
// Fin test SFML

#include "state.h"
#include "render.h"
#include "engine.h"
#include "ai.h"

using namespace std;
using namespace state;
using namespace render;
using namespace engine;
using namespace ai;

int main(int argc,char* argv[]){

	testSFML();
		
	if (argc>1){
		/*	hello : Affichage simple */
		if(strcmp(argv[1],"hello")==0){
			cout<<"Bonjour tout le monde"<<endl;
		}

		else if(strcmp(argv[1],"random_ai")==0 || strcmp(argv[1],"heuristic_ai")==0){
			
			unsigned int longueur_map_cases = 25, largeur_map_cases = 25;
			std::string chemin_fichier_map_txt = "res/map1.txt";
			
			Correspondances tab_corres = Correspondances();
			
			//-----------------------------
			Moteur moteur;
			
			if(	moteur.getEtat().initGrille(chemin_fichier_map_txt, longueur_map_cases, largeur_map_cases, tab_corres)){
				moteur.getEtat().initPersonnages(tab_corres);
				moteur.getEtat().initCurseur();
				StateLayer stateLayer(moteur.getEtat());
				stateLayer.initSurfaces(moteur.getEtat());
				
				//-----------------------------
				StateLayer* ptr_stateLayer=&stateLayer;
				moteur.getEtat().registerObserver(ptr_stateLayer);

				//-----------------------------
				
				sf::RenderWindow window(sf::VideoMode(longueur_map_cases*stateLayer.getTilesets()[0]->getCellHeight(),
													  largeur_map_cases*stateLayer.getTilesets()[0]->getCellWidth()),
													  "Map");

				
				
				RandomIA random_ai;
				HeuristicIA heuristic_ai;
				
				if(strcmp(argv[1],"heuristic_ai")==0){
					cout<<"\t\t--- HeuristicAI ---"<<endl;
				}
				else if(strcmp(argv[1],"random_ai")==0){
					cout<<"\t\t--- RandomAI ---"<<endl;
				}
				
				bool demarrage = true ;
				int changementX = 0, changementY = 0;
				
				
				while (window.isOpen()){
					sf::Event event;
					
					// Appel à l'IA choisie pour le tour adverse
					if(strcmp(argv[1],"heuristic_ai")==0){
						heuristic_ai.run(moteur, window);
					}
					else {
						random_ai.run(moteur, window);
					}
					// Verication de fin de tour et reinitialisations de debut de tour
					if(!moteur.getEtat().getFin() && moteur.verificationFinDeTour()){
								moteur.verificationDebutDeTour();
								StateEvent majDisponibilite(ALLCHANGED);
								moteur.getEtat().notifyObservers(majDisponibilite, moteur.getEtat(), window);
					}
					
					// Action au demarrage du jeu
					if (demarrage){
						stateLayer.draw(window);						
						cout << "\n\t\t--- Tour " << moteur.getEtat().getTour() << " ---\n" << endl;
						cout << "***********************************************************" << endl;
						cout << "*                       CONTROLES                         *" << endl;
						cout << "*  Déplacer le curseur         : flèches directionnelles  *" << endl;
						cout << "*  Sélectionner Personnage     : touche Enter             *" << endl;
						cout << "*                                                         *" << endl;
						cout << "*  Déclencher une Attaque      : touche A                 *" << endl;
						cout << "*  Choisir cible d'attaque     : flèches directionnelles  *" << endl;
						cout << "*  Valider attaque             : touche Enter             *" << endl;
						cout << "*  Annuler Attaque             : touche N                 *" << endl;
						cout << "*                                                         *" << endl;
						cout << "*  Terminer le tour d'actions  : touche Z                 *" << endl;
						cout << "***********************************************************" << endl;
						demarrage = false;
					}
					
					while (window.pollEvent(event)){
						// Fermeture de la fenetre
						if (event.type == sf::Event::Closed){
							window.close();
						}
						
						
						/*  CURSEUR (WIP : code a deplacer par la suite) */
						
						// Appui d'une flèche directionnelle ou de Enter sans selection 
						// (moteur.getEtat().verifStatut()!=-1)
						else if(event.type==sf::Event::KeyPressed && (moteur.getEtat().verifStatut()==-1)){
						
							StateEvent stateEvent(ALLCHANGED);
							size_t xCurs=moteur.getEtat().getCurseur()->getPosition().getX();
							size_t yCurs=moteur.getEtat().getCurseur()->getPosition().getY();
							int numeroPerso = moteur.getEtat().getGrille()[xCurs][yCurs]->isOccupe(moteur.getEtat());
							
							// Fleches directionnelles (deplacement du curseur)
							if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
								if(xCurs!=largeur_map_cases-1){changementX = 1;}
								else{changementX = -xCurs;}
							}
							else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
								if(xCurs!=0){changementX = -1;}
								else{changementX = largeur_map_cases-1 -xCurs;}
							}
							else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
								if(yCurs!=0){changementY = -1;}
								else{changementY = longueur_map_cases-1 -yCurs;}
							}
							else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
								if(yCurs!=longueur_map_cases-1){changementY = 1;}
								else{changementY = -yCurs;}
							}
							
							// Enter (selection)
							else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)){								
								if(numeroPerso != -1){
									// Selection du personnage
									if(moteur.getEtat().getPersonnages()[numeroPerso]->getCamp()){
										moteur.getEtat().getCurseur()->setCodeTuile(3);
										cout<< "\t\t-> Début du tour de " << moteur.getEtat().getPersonnages()[numeroPerso]->getNom() << " <-" << endl;
										moteur.getEtat().getPersonnages()[numeroPerso]->setStatut(SELECTIONNE);
										StateEvent stateEvent(ALLCHANGED);
										moteur.getEtat().notifyObservers(stateEvent, moteur.getEtat(), window);
									}									
									else{	cout<<"Ce personnage appartient à l'adversaire !" <<endl;}
								}
								// Affichage du type de terrain							
								else{	cout<< "Ceci est une : ";
										cout << moteur.getEtat().getGrille()[yCurs][xCurs]->getNom() << endl;
								}
							}
							
							// Déplacement du curseur
							if (changementX != 0 || changementY !=0){
								Position nextPosCurs(xCurs+changementX, yCurs+changementY);
								moteur.getEtat().getCurseur()->move(nextPosCurs);
								moteur.getEtat().notifyObservers(stateEvent, moteur.getEtat(), window);
								
								changementX = 0;
								changementY = 0;
							}
						}
						
						// Actions avec personnage selectionne (moteur.getEtat().verifStatut()!=-1)
						else if(event.type==sf::Event::KeyPressed && (moteur.getEtat().verifStatut()!=-1)){
							int numeroPerso=moteur.getEtat().verifStatut();
							size_t xPerso=moteur.getEtat().getPersonnages()[numeroPerso]->getPosition().getX();
							size_t yPerso=moteur.getEtat().getPersonnages()[numeroPerso]->getPosition().getY();
							
							// Fleches directionnelles (deplacement du curseur et du personnage)
							if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
								if(xPerso!=largeur_map_cases-1){changementX = 1;}
							}
							else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
								if(xPerso!=0){changementX = -1;}
							}
							else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
								if(yPerso!=0){changementY = -1;}
							}
							else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
								if(yPerso!=longueur_map_cases-1){changementY = 1;}
							}
							
							// Touche Z : Fin de tour d'actions
							else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)){							
								FinActions finaction(*moteur.getEtat().getPersonnages()[numeroPerso],moteur.getEtat().getPersonnages()[numeroPerso]->getCamp());
								unique_ptr<Commande> ptr_finaction (new FinActions(finaction));
								moteur.addCommande(0, move(ptr_finaction));								
								moteur.getEtat().getCurseur()->setCodeTuile(0);
								moteur.update(window);
							}
							
							
							
							// Deplacement du curseur et du personnage selectionne
							if (changementX != 0 || changementY != 0){
								Position nextPos(xPerso+changementX, yPerso+changementY);
								Deplacement deplacement(*moteur.getEtat().getPersonnages()[numeroPerso], nextPos, moteur.getJoueurActif());
								unique_ptr<Commande> ptr_deplacement (new Deplacement(deplacement));
								moteur.addCommande(0, move(ptr_deplacement));
								
								moteur.getEtat().getCurseur()->move(nextPos);
								moteur.update(window);
								
								changementX = 0;
								changementY = 0;
							}
						}
						
						// Declenchement d'un attaque
						else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && (moteur.getEtat().verifStatut()!=-1)){
							cout<< "\tAttaque en préparation" << endl;
							
							StateEvent stateEvent(ALLCHANGED);
							int attaquant=moteur.getEtat().verifStatut();
							int champDroit=moteur.getEtat().getPersonnages()[attaquant]->getChampAttack();
							int champGauche=champDroit;
							int champHaut=champDroit;
							int champBas=champDroit;
							int cible=-1;
							
							moteur.getEtat().getCurseur()->setCodeTuile(1);
							moteur.getEtat().notifyObservers(stateEvent, moteur.getEtat(), window);
							
							while((cible==-1) || (cible==attaquant)){								
								if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
									size_t xCurs=moteur.getEtat().getCurseur()->getPosition().getX();
									size_t yCurs=moteur.getEtat().getCurseur()->getPosition().getY();
									if(xCurs!=largeur_map_cases-1 && champDroit!=0){
										Position nextPosCurs(xCurs+1, yCurs);
										
										moteur.getEtat().getCurseur()->move(nextPosCurs);
										champDroit=champDroit-1;
										champGauche++;	
										moteur.getEtat().notifyObservers(stateEvent, moteur.getEtat(), window);
										usleep(200000);
									}
								}
								else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
									size_t xCurs=moteur.getEtat().getCurseur()->getPosition().getX();
									size_t yCurs=moteur.getEtat().getCurseur()->getPosition().getY();
									if(xCurs!=0 && champGauche!=0){
										Position nextPosCurs(xCurs-1, yCurs);	
										moteur.getEtat().getCurseur()->move(nextPosCurs);
										champGauche=champGauche-1;
										champDroit++;
										moteur.getEtat().notifyObservers(stateEvent, moteur.getEtat(), window);
										usleep(200000);
									}
								}
								else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
									size_t xCurs=moteur.getEtat().getCurseur()->getPosition().getX();
									size_t yCurs=moteur.getEtat().getCurseur()->getPosition().getY();
									if(yCurs!=longueur_map_cases-1 && champBas!=0){
										Position nextPosCurs(xCurs, yCurs+1);	
										moteur.getEtat().getCurseur()->move(nextPosCurs);
										champBas=champBas-1;
										champHaut++;
										moteur.getEtat().notifyObservers(stateEvent, moteur.getEtat(), window);
										usleep(200000);
									}
								}
								else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
									size_t xCurs=moteur.getEtat().getCurseur()->getPosition().getX();
									size_t yCurs=moteur.getEtat().getCurseur()->getPosition().getY();
									if(yCurs!=0 && champHaut!=0){
										Position nextPosCurs(xCurs, yCurs-1);
										moteur.getEtat().getCurseur()->move(nextPosCurs);
										champHaut=champHaut-1;
										champBas++;
										moteur.getEtat().notifyObservers(stateEvent, moteur.getEtat(), window); 
										usleep(200000);
									}
								}
								
								// Annulation de l'attaque avec N
								else if(sf::Keyboard::isKeyPressed(sf::Keyboard::N)){
									cible=-2;
									cout<< "\tAttaque annulée" << endl;									
									moteur.getEtat().getCurseur()->setCodeTuile(3);
									moteur.getEtat().notifyObservers(stateEvent, moteur.getEtat(), window);				
									if(!moteur.getEtat().getCurseur()->getPosition().equals(moteur.getEtat().getPersonnages()[attaquant]->getPosition())){
										Position nextPosCurs(moteur.getEtat().getPersonnages()[attaquant]->getPosition().getX(), moteur.getEtat().getPersonnages()[attaquant]->getPosition().getY());
										moteur.getEtat().getCurseur()->move(nextPosCurs);
									
										moteur.getEtat().notifyObservers(stateEvent, moteur.getEtat(), window);
									}
								}
								
								// Validation de l'attaque avec Enter
								else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return)){
									cible=moteur.getEtat().getGrille()[moteur.getEtat().getCurseur()->getPosition().getX()][moteur.getEtat().getCurseur()->getPosition().getY()]->isOccupe(moteur.getEtat());
								}
							}
							
							// Validation de l'attaque
							if(cible>=0){
								Attaque attaque(*moteur.getEtat().getPersonnages()[attaquant], *moteur.getEtat().getPersonnages()[cible],moteur.getEtat().getPersonnages()[attaquant]->getCamp());
								unique_ptr<Commande> ptr_attaque (new Attaque(attaque));
								moteur.addCommande(0, move(ptr_attaque));
								
								moteur.getEtat().getCurseur()->setCodeTuile(2);
								moteur.getEtat().notifyObservers(stateEvent, moteur.getEtat(), window);				
								moteur.update(window);
								moteur.getEtat().getCurseur()->setCodeTuile(0);
								moteur.getEtat().notifyObservers(stateEvent, moteur.getEtat(), window);									
							}
						}
					}
				}	
			}
		}
		
		else if(strcmp(argv[1],"engine")==0){
			cout<<"--- Moteur du jeu ---"<<endl;
			//----------------------------
			unsigned int longueur_map_cases = 25, largeur_map_cases = 25;
			std::string chemin_fichier_map_txt = "res/map1.txt";
			
			Correspondances tab_corres = Correspondances();
			
			//-----------------------------
			Moteur moteur;
			
			if(	moteur.getEtat().initGrille(chemin_fichier_map_txt, longueur_map_cases, largeur_map_cases, tab_corres)){
				moteur.getEtat().initPersonnages(tab_corres);
				moteur.getEtat().initCurseur();
				StateLayer stateLayer(moteur.getEtat());
				stateLayer.initSurfaces(moteur.getEtat());
				
				//----------------------------
				StateLayer* ptr_stateLayer=&stateLayer;
				moteur.getEtat().registerObserver(ptr_stateLayer);

				//------------------------
				
				sf::RenderWindow window(sf::VideoMode(longueur_map_cases*stateLayer.getTilesets()[0]->getCellHeight(),
													  largeur_map_cases*stateLayer.getTilesets()[0]->getCellWidth()),
													  "Map");

				bool demarrage = true;
				
				while (window.isOpen()){
					sf::Event event;
					
					if (demarrage){
						stateLayer.draw(window);
						cout << "\t\t--- Tour " << moteur.getEtat().getTour() << " ---" << endl;
						cout << "(Appuyez sur une touche pour simuler un tour de jeu)" << endl;
						cout << "(Cette simulation compte 3 tours)\n" << endl;
						demarrage = false;
					}
					
					while (window.pollEvent(event)){
						if (event.type == sf::Event::Closed){
							window.close();
						}
						
						// Commandes du premier tour pour la simulation (effectuees a l'appuis d'une touche)
						else if(event.type==sf::Event::KeyPressed && !moteur.getEtat().getFin() && moteur.getEtat().getTour() == 1){
												
							// Deplacement chevalier bleu
							Position destination1(3,22);
							Deplacement deplacement1(*moteur.getEtat().getPersonnages()[2], destination1,true);
							unique_ptr<Commande> ptr_deplacement1 (new Deplacement(deplacement1));
							moteur.addCommande(0, move(ptr_deplacement1));
							
							// Attaque chevalier bleu contre archer rouge
							Attaque attaque1(*moteur.getEtat().getPersonnages()[2], *moteur.getEtat().getPersonnages()[4],true);
							unique_ptr<Commande> ptr_attaque1 (new Attaque(attaque1));
							moteur.addCommande(1, move(ptr_attaque1));
							
							// Deplacement chevalier bleu
							Position destination2(1,20);
							Deplacement deplacement2(*moteur.getEtat().getPersonnages()[0], destination2,true);
							unique_ptr<Commande> ptr_deplacement2 (new Deplacement(deplacement2));
							moteur.addCommande(2, move(ptr_deplacement2));
							
							// Fin tour archer bleu
							FinActions finactions1(*moteur.getEtat().getPersonnages()[0],true);
							unique_ptr<Commande> ptr_finactions1 (new FinActions(finactions1));
							moteur.addCommande(3, move(ptr_finactions1));
							
							// Premier deplacement du brigand bleu
							Position destination3(3,20);
							Deplacement deplacement3(*moteur.getEtat().getPersonnages()[1], destination3,true);
							unique_ptr<Commande> ptr_deplacement3 (new Deplacement(deplacement3));
							moteur.addCommande(4, move(ptr_deplacement3));
							
							// Deuxieme deplacement du brigand bleu
							Position destination4(4,20);
							Deplacement deplacement4(*moteur.getEtat().getPersonnages()[1], destination4,true);
							unique_ptr<Commande> ptr_deplacement4 (new Deplacement(deplacement4));
							moteur.addCommande(5, move(ptr_deplacement4));
							
							// Troisieme deplacement du brigand bleu
							Position destination5(4,19);
							Deplacement deplacement5(*moteur.getEtat().getPersonnages()[1], destination5,true);
							unique_ptr<Commande> ptr_deplacement5 (new Deplacement(deplacement5));
							moteur.addCommande(6, move(ptr_deplacement5));
							
							// Tentative de quatrieme deplacement du brigand bleu
							Position destination6(4,18);
							Deplacement deplacement6(*moteur.getEtat().getPersonnages()[1], destination6,true);
							unique_ptr<Commande> ptr_deplacement6 (new Deplacement(deplacement6));
							moteur.addCommande(7, move(ptr_deplacement6));
														
							// Fin tour brigand bleu
							FinActions finactions2(*moteur.getEtat().getPersonnages()[1],true);
							unique_ptr<Commande> ptr_finactions2 (new FinActions(finactions2));
							moteur.addCommande(8, move(ptr_finactions2));
							
							// Fin tour guerrier bleu
							FinActions finactions3(*moteur.getEtat().getPersonnages()[3],true);
							unique_ptr<Commande> ptr_finactions3 (new FinActions(finactions3));
							moteur.addCommande(9, move(ptr_finactions3));
						
							moteur.update(window);							
														
							if(moteur.verificationFinDeTour()){
								moteur.verificationDebutDeTour();
								StateEvent majDisponibilite(ALLCHANGED);
								moteur.getEtat().notifyObservers(majDisponibilite, moteur.getEtat(), window);
							}
						}
						
						// Commandes du deuxieme tour pour la simulation
						else if(event.type==sf::Event::KeyPressed && !moteur.getEtat().getFin() && moteur.getEtat().getTour() == 2){
						
							// Fin tour archer rouge
							FinActions finactions4(*moteur.getEtat().getPersonnages()[4],false);
							unique_ptr<Commande> ptr_finactions4 (new FinActions(finactions4));
							moteur.addCommande(0, move(ptr_finactions4));
							
							// Tentative d'attaque archer rouge contre chevalier bleu
							Attaque attaque2(*moteur.getEtat().getPersonnages()[4], *moteur.getEtat().getPersonnages()[2],false);
							unique_ptr<Commande> ptr_attaque2 (new Attaque(attaque2));
							moteur.addCommande(1, move(ptr_attaque2));
						
							moteur.update(window);							
														
							if(moteur.verificationFinDeTour()){
								moteur.verificationDebutDeTour();
								StateEvent majDisponibilite(ALLCHANGED);
								moteur.getEtat().notifyObservers(majDisponibilite, moteur.getEtat(), window);
							}
						}
						
						// Commandes du troisieme tour pour la simulation
						else if(event.type==sf::Event::KeyPressed && !moteur.getEtat().getFin() && moteur.getEtat().getTour() == 3){
						
							// Quatrieme deplacement du brigand bleu
							Position destination7(4,18);
							Deplacement deplacement7(*moteur.getEtat().getPersonnages()[1], destination7,true);
							unique_ptr<Commande> ptr_deplacement7 (new Deplacement(deplacement7));
							moteur.addCommande(0, move(ptr_deplacement7));
							
							// Cinquieme deplacement du brigand bleu
							Position destination8(3,18);
							Deplacement deplacement8(*moteur.getEtat().getPersonnages()[1], destination8,true);
							unique_ptr<Commande> ptr_deplacement8 (new Deplacement(deplacement8));
							moteur.addCommande(1, move(ptr_deplacement8));
							
							// Tentative fin de tour archer rouge
							FinActions finactions5(*moteur.getEtat().getPersonnages()[4],false);
							unique_ptr<Commande> ptr_finactions5 (new FinActions(finactions5));
							moteur.addCommande(2, move(ptr_finactions5));
							
							// Fin tour brigand bleu
							FinActions finactions6(*moteur.getEtat().getPersonnages()[1],true);
							unique_ptr<Commande> ptr_finactions6 (new FinActions(finactions6));
							moteur.addCommande(3, move(ptr_finactions6));
							
							// Deuxieme deplacement de l'archer bleu
							Position destination9(1,21);
							Deplacement deplacement9(*moteur.getEtat().getPersonnages()[0], destination9,true);
							unique_ptr<Commande> ptr_deplacement9 (new Deplacement(deplacement9));
							moteur.addCommande(4, move(ptr_deplacement9));
							
							// Attaque archer bleu contre guerrier rouge
							Attaque attaque3(*moteur.getEtat().getPersonnages()[0], *moteur.getEtat().getPersonnages()[4],true);
							unique_ptr<Commande> ptr_attaque3 (new Attaque(attaque3));
							moteur.addCommande(5, move(ptr_attaque3));
							
							// Attaque chevalier bleu contre guerrier rouge
							Attaque attaque4(*moteur.getEtat().getPersonnages()[2], *moteur.getEtat().getPersonnages()[4],true);
							unique_ptr<Commande> ptr_attaque4 (new Attaque(attaque4));
							moteur.addCommande(6, move(ptr_attaque4));
							
							// Fin tour guerrier bleu
							FinActions finactions7(*moteur.getEtat().getPersonnages()[3],true);
							unique_ptr<Commande> ptr_finactions7 (new FinActions(finactions7));
							moteur.addCommande(7, move(ptr_finactions7));
						
							moteur.update(window);							
														
							if(moteur.verificationFinDeTour()){
								moteur.verificationDebutDeTour();
								StateEvent majDisponibilite(ALLCHANGED);
								moteur.getEtat().notifyObservers(majDisponibilite, moteur.getEtat(), window);
							}
						}	
					}
					
					/*
					if (tourSimule == 1){
						FinActions finattaque12(*moteur.getEtat().getPersonnages()[4],false);
						unique_ptr<Commande> ptr_finactions12 (new FinActions(finattaque12));
						moteur.addCommande(0, move(ptr_finactions12));
						
							FinActions finattaque5(*moteur.getEtat().getPersonnages()[4],false);
							unique_ptr<Commande> ptr_finactions5 (new FinActions(finattaque5));
							moteur.addCommande(0, move(ptr_finactions5));
							
							FinActions finattaque6(*moteur.getEtat().getPersonnages()[5],false);
							unique_ptr<Commande> ptr_finactions6 (new FinActions(finattaque6));
							moteur.addCommande(1, move(ptr_finactions6));
							
							FinActions finattaque7(*moteur.getEtat().getPersonnages()[6],false);
							unique_ptr<Commande> ptr_finactions7 (new FinActions(finattaque7));
							moteur.addCommande(2, move(ptr_finactions7));
							
							FinActions finattaque8(*moteur.getEtat().getPersonnages()[7],false);
							unique_ptr<Commande> ptr_finactions8 (new FinActions(finattaque8));
							moteur.addCommande(3, move(ptr_finactions8));
							
							Position destination3(16,5);
							Deplacement deplacement3(*moteur.getEtat().getPersonnages()[7], destination3,false);
							unique_ptr<Commande> ptr_deplacement3 (new Deplacement(deplacement3));
							moteur.addCommande(4, move(ptr_deplacement3));
					}
					
					
					else if(tourSimule==0){
						tourSimule=1;
												
						
						
						
						// Pas d'actions pour les autres personnages bleus
						
																		
						FinActions finattaque1(*moteur.getEtat().getPersonnages()[1],true);
						unique_ptr<Commande> ptr_finactions1 (new FinActions(finattaque1));
						moteur.addCommande(3, move(ptr_finactions1));
						
						// Action du joueur adverse alors que ce n'est pas son tour
						FinActions finattaque2(*moteur.getEtat().getPersonnages()[5],false);
						unique_ptr<Commande> ptr_finactions2 (new FinActions(finattaque2));
						moteur.addCommande(4, move(ptr_finactions2));
												
						FinActions finattaque4(*moteur.getEtat().getPersonnages()[3],true);
						unique_ptr<Commande> ptr_finactions4 (new FinActions(finattaque4));
						moteur.addCommande(5, move(ptr_finactions4));
						
						
					}*/
				}
			}
		}


		/*	render : Affichage du rendu de la map */
		else if(strcmp(argv[1],"render")==0){
		
			cout<<"--- Affichage d'un Etat ---"<<endl;
			
			unsigned int longueur_map_cases = 25, largeur_map_cases = 25;
			std::string chemin_fichier_map_txt = "res/map1.txt";
			
			Correspondances tab_corres = Correspondances();
			
			// -- Initialisation d'un etat --
			Etat etat_initial;			
			if(etat_initial.initGrille(chemin_fichier_map_txt, longueur_map_cases, largeur_map_cases, tab_corres)){
			cout << "Taille grille : " <<etat_initial.getGrille().size() << "x" << etat_initial.getGrille()[0].size() << endl;
			etat_initial.initPersonnages(tab_corres);
			etat_initial.initCurseur();
			cout << "Taille liste de personnages : " << etat_initial.getPersonnages().size() << endl;
			
			// -- Affichage de cet Etat --
			StateLayer layer(etat_initial);			
			// La variable layer.getTilesets()[0] est le tileset de la grille
			sf::RenderWindow window(sf::VideoMode(	longueur_map_cases*layer.getTilesets()[0]->getCellHeight(),
													largeur_map_cases*layer.getTilesets()[0]->getCellWidth()),
													"Map");
			
			layer.initSurfaces(etat_initial);
	
			while (window.isOpen()){
				sf::Event event;
				while (window.pollEvent(event)){
					if (event.type == sf::Event::Closed){
						window.close();
					}
				}
				
				window.clear();
				
				window.draw(*layer.getSurfaces()[0]);	// Dessin de la grille				
				window.draw(*layer.getSurfaces()[1]);	// Dessin des personnages
				window.display();
			}
			}
		}

		/*	state : Tests unitaires*/
		else if(strcmp(argv[1],"state")==0){
					
			Personnage perso = Personnage(ARCHER,true, "archerTest",0, 0);
			
			int count_err = 0;
			
			cout<<"--- Tests unitaires ---"<<endl;
			
			perso.getStatistiques().setPV(-5);
			perso.getStatistiques().setAttaque(-5);
			perso.getStatistiques().setDefense(-5);
			perso.getStatistiques().setEsquive(-5);
			perso.getStatistiques().setCritique(-5);
			
			if (perso.getStatistiques().getPV() < 0){
				cerr << "Warning : Attribut PV negatif interdit" <<endl;
				count_err++;
			}
			if (perso.getStatistiques().getAttaque() < 0){
				cerr << "Warning : Attribut Attaque negatif interdit" <<endl;
				count_err++;
			}
			if (perso.getStatistiques().getDefense() < 0){
				cerr << "Warning : Attribut Defense negatif interdit" <<endl;
				count_err++;
			}
			if (perso.getStatistiques().getEsquive() < 0){
				cerr << "Warning : Attribut Esquive negatif interdit" <<endl;
				count_err++;
			}
			if (perso.getStatistiques().getCritique() < 0){
				cerr << "Warning : Attribut Critique negatif interdit" <<endl;
				count_err++;
			}
			
			cout << "Tests unitaires termines avec " << count_err << " erreur(s)" <<endl;
			
			
		}
		else{
			exit(EXIT_FAILURE);
		}
	}
    return 0;
}
