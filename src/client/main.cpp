#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <map>
#include <memory>
#include <unistd.h>
#include <thread>

// Les lignes suivantes ne servent qu'à vérifier que la compilation avec SFML fonctionne
#include <SFML/Graphics.hpp>
void testSFML() {
    sf::Texture texture;
}
// Fin test SFML

//#include "../../extern/jsoncpp-1.8.0/jsoncpp.cpp"

#include "state.h"
#include "render.h"
#include "engine.h"
#include "ai.h"
#include "client.h"

using namespace std;
using namespace state;
using namespace render;
using namespace engine;
using namespace ai;
using namespace client;


int main(int argc,char* argv[]){

	testSFML();
		
	if (argc>1){
		/*	hello : Affichage simple */
		if(strcmp(argv[1],"hello")==0){
			cout<<"Bonjour tout le monde"<<endl;
		}
		
		/* play : lecture d'une partie enregistrée */
		else if(strcmp(argv[1], "play") == 0){
			Json::Value root;
			unsigned int longueur_map_cases = 25, largeur_map_cases = 25;
			std::string chemin_fichier_map_txt = "res/map1.txt";
			std::string fichier_commandes = "res/replay.txt";
			
			// Creation des tables de correspondances et du moteur
			Correspondances tab_corres = Correspondances();
			Moteur moteur;
			
			if(	moteur.getEtat().initGrille(chemin_fichier_map_txt, longueur_map_cases, largeur_map_cases, tab_corres)){
				sf::RenderWindow window(sf::VideoMode(largeur_map_cases*16,longueur_map_cases*16 +200),"Map");
				
				moteur.getEtat().initPersonnages(tab_corres);
				moteur.getEtat().initCurseur();
				StateLayer stateLayer(moteur.getEtat(), window);
				stateLayer.initSurfaces(moteur.getEtat());
								
				StateLayer* ptr_stateLayer=&stateLayer;
				moteur.getEtat().registerObserver(ptr_stateLayer);
				
				
								
				cout << "\t\t--- Play ---" << endl;
				
				bool demarrage = true ;
				
				//Ouverture du fichier en lecture
				std::ifstream commandes_txt(fichier_commandes);
				
				// Si le fichier s'est bien ouvert
				if (commandes_txt){
					Json::Value root;
					Json::Reader reader;

					if(!reader.parse(commandes_txt, root)){
						cout 	<< "Failed to parse commandes\n"
								<< reader.getFormattedErrorMessages();
						return 0;
					}
					// Fermeture du fichier
					commandes_txt.close();
					
					// Conversion en commandes
					cout << "Taille du tableau de commandes de "<< fichier_commandes << " : " << root["tabCmd"].size() << endl;
					
					Position buf_destination(0,0);
					
					for (unsigned int i = 0; i < root["tabCmd"].size(); i++){
						if(root["tabCmd"][i]["id"].asUInt() == 1){
							buf_destination.setX(root["tabCmd"][i]["xDestination"].asUInt());
							buf_destination.setY(root["tabCmd"][i]["yDestination"].asUInt());
							Deplacement deplacement(*moteur.getEtat().getPersonnages()[root["tabCmd"][i]["cible"].asUInt()], buf_destination, root["tabCmd"][i]["joueur"].asUInt());
							
							unique_ptr<Commande> ptr_deplacement (new Deplacement(deplacement));
							moteur.addCommande(i, move(ptr_deplacement));
						}
						else if(root["tabCmd"][i]["id"].asUInt() == 2){
							Attaque attaque(*moteur.getEtat().getPersonnages()[root["tabCmd"][i]["attaquant"].asUInt()], *moteur.getEtat().getPersonnages()[root["tabCmd"][i]["cible"].asUInt()], root["tabCmd"][i]["joueur"].asUInt());
							
							unique_ptr<Commande> ptr_attaque (new Attaque(attaque));
							moteur.addCommande(i, move(ptr_attaque));
						}
						else if(root["tabCmd"][i]["id"].asUInt() == 3){
							FinActions finactions(*moteur.getEtat().getPersonnages()[root["tabCmd"][i]["cible"].asUInt()],root["tabCmd"][i]["joueur"].asUInt());
							unique_ptr<Commande> ptr_finactions (new FinActions(finactions));
							moteur.addCommande(i, move(ptr_finactions));
						}
						else{
							cout << "La commande " << i << " est inconnue" << endl;
						}						
					}
					cout << "Pour lancer la partie, appuyez sur la touche P\n" << endl;														
				}
				
				else{
					cerr << "Impossible d'ouvrir le fichier des commandes enregistrées (lecture)." << endl;
					return 0;
				}
				
				bool partie_rejouee = false;
				sf::Event event;
				
				while (window.isOpen()){				
					// Appel à l'IA choisie pour le tour adverse
					if(partie_rejouee == false && sf::Keyboard::isKeyPressed(sf::Keyboard::P)){
						cout << "--> Debut de la Lecture <--\n" << endl;
						moteur.update();
						cout << "--> Lecture Terminée <--" << endl;
						partie_rejouee = true;
					}
								
					if (demarrage){
						stateLayer.draw(window);
						demarrage = false;
					}
					
					while (window.pollEvent(event)){
						// Fermeture de la fenetre
						if (event.type == sf::Event::Closed){
							window.close();
						}
					}					
				}
			}
		}
		
		/*	thread : le moteur tourne dans un thread séparé*/
		else if(strcmp(argv[1], "thread") == 0){
			cout << "\t\t--- thread ---" << endl;
			
			unsigned int longueur_map_cases = 25, largeur_map_cases = 25;
			sf::RenderWindow window(sf::VideoMode(largeur_map_cases*16, longueur_map_cases*16 + 200),"Map");
	
			Client client(window); //un client un est un observer - observateur du moteur de jeu
			
			while (window.isOpen()){
				
				client.run();
				sleep(2);
				window.close();									
						
			}

		}
		else if(strcmp(argv[1], "record") == 0){
			unsigned int longueur_map_cases = 25, largeur_map_cases = 25;
			std::string chemin_fichier_map_txt = "res/map1.txt";
			
			// Creation des tables de correspondances et du moteur
			Correspondances tab_corres = Correspondances();
			Moteur moteur;
			
			
			if(	moteur.getEtat().initGrille(chemin_fichier_map_txt, longueur_map_cases, largeur_map_cases, tab_corres)){
				moteur.getEtat().initPersonnages(tab_corres);
				moteur.getEtat().initCurseur();

				sf::RenderWindow window(sf::VideoMode(400,400+200),"Map");
				StateLayer stateLayer(moteur.getEtat(), window);
				stateLayer.initSurfaces(moteur.getEtat());
								
				StateLayer* ptr_stateLayer=&stateLayer;
				moteur.getEtat().registerObserver(ptr_stateLayer);
				Moteur* ptr_moteur=&moteur;
				stateLayer.registerObserver(ptr_moteur);
				
				//sf::RenderWindow window(sf::VideoMode(largeur_map_cases*stateLayer.getTilesets()[0]->getCellHeight(),longueur_map_cases*stateLayer.getTilesets()[0]->getCellWidth()+200),"Map");
				
				
				HeuristicIA armeeRouge;
				HeuristicIA armeeBleue;
			
				armeeBleue.setCamp(true);
				
				cout << "\t\t--- Record ---" << endl;
				
				bool demarrage = true ;
				
				while (window.isOpen()){	

					/*
					//gestion des evenements clic souris bouton
					render->handleEvent();
					//gestion de l'ia
					ia->run();
					//gestion de l'engine
					engine->update();
					//update de l'affichage
					render->update();	*/
						
					sf::Event event;									
					// Verication de fin de tour et reinitialisations de debut de tour
					if(!moteur.getEtat().getFin() && moteur.verificationFinDeTour()){
								moteur.verificationDebutDeTour();
								StateEvent majDisponibilite(ALLCHANGED);
								moteur.getEtat().notifyObservers(majDisponibilite, moteur.getEtat());
					}
					if (demarrage){
						stateLayer.draw(window);
											
						cout << "\n\t\t--- Tour " << moteur.getEtat().getTour() << " ---\n" << endl;
						
						demarrage = false;
					}
					
					// Appel à l'IA choisie pour le tour adverse
					if(moteur.getJoueurActif() == true && sf::Keyboard::isKeyPressed(sf::Keyboard::H)){
						armeeBleue.run(moteur);
					}
					else if(moteur.getJoueurActif() == false && sf::Keyboard::isKeyPressed(sf::Keyboard::H)){
						armeeRouge.run(moteur);
					}
					
					while (window.pollEvent(event)){
						// Fermeture de la fenetre
						if (event.type == sf::Event::Closed){
							window.close();
						}
					}					
				}
			}
		} 

		else if(strcmp(argv[1],"random_ai")==0 || strcmp(argv[1],"heuristic_ai")==0 || strcmp(argv[1], "rollback") == 0 || strcmp(argv[1],"deep_ai")==0){
			
			unsigned int longueur_map_cases = 25, largeur_map_cases = 25;
			std::string chemin_fichier_map_txt = "res/map1.txt";
			
			// Creation des tables de correspondances et du moteur
			Correspondances tab_corres = Correspondances();
			Moteur moteur;
			
			// Lorsque la map est bien cargee
			if(	moteur.getEtat().initGrille(chemin_fichier_map_txt, longueur_map_cases, largeur_map_cases, tab_corres)){
				moteur.getEtat().initPersonnages(tab_corres);
				moteur.getEtat().initCurseur();

				sf::RenderWindow window(sf::VideoMode(400,400+200),"Map");
				StateLayer stateLayer(moteur.getEtat(), window);
				stateLayer.initSurfaces(moteur.getEtat());
								
				StateLayer* ptr_stateLayer=&stateLayer;
				moteur.getEtat().registerObserver(ptr_stateLayer);
				Moteur* ptr_moteur=&moteur;
				stateLayer.registerObserver(ptr_moteur);
				
				//sf::RenderWindow window(sf::VideoMode(largeur_map_cases*stateLayer.getTilesets()[0]->getCellHeight(),longueur_map_cases*stateLayer.getTilesets()[0]->getCellWidth()+200),"Map");

				
				
				RandomIA random_ai;
				HeuristicIA heuristic_ai;
				DeepIA deep_ai;
				bool rollback = false;
				
				if(strcmp(argv[1],"heuristic_ai")==0){
					cout<<"\t\t--- HeuristicAI ---"<<endl;
				}
				else if(strcmp(argv[1],"random_ai")==0){
					cout<<"\t\t--- RandomAI ---"<<endl;
				}
				else if(strcmp(argv[1],"deep_ai")==0){
					cout<<"\t\t--- DeepAI ---"<<endl;
				}
				else if(strcmp(argv[1],"rollback")==0){
					cout<<"\t\t--- Rollback ---"<<endl;
					rollback = true;
				}
				
				bool demarrage = true ;				
				
				while (window.isOpen()){
				
					sf::Event event;
					
					// Verication de fin de tour et reinitialisations de debut de tour
					if(!moteur.getEtat().getFin() && moteur.verificationFinDeTour()){
								moteur.verificationDebutDeTour();
								StateEvent majDisponibilite(ALLCHANGED);
								moteur.getEtat().notifyObservers(majDisponibilite, moteur.getEtat());
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
						
						if(rollback){
							cout<<"\nAppuyez sur R lors de votre tour pour déclencher le retour en arrière\n"<< endl;
						}
						
						demarrage = false;
					}
					
					// Appel à l'IA choisie pour le tour adverse
					if(strcmp(argv[1],"heuristic_ai")==0 || strcmp(argv[1],"rollback")==0){
						if(rollback){
							heuristic_ai.runRollback(moteur);
						}
						else{
							heuristic_ai.run(moteur);
						}
					}
					else if (strcmp(argv[1],"deep_ai")==0){
						deep_ai.run(moteur);
					}
					else {
						random_ai.run(moteur);
					}
					
					while (window.pollEvent(event)){
						// Fermeture de la fenetre
						if (event.type == sf::Event::Closed){
							window.close();
						}
						
						else if (event.type==sf::Event::KeyPressed && moteur.getEtat().getFin() == false && rollback){
							stateLayer.gestionCurseur(event, largeur_map_cases, longueur_map_cases, moteur.getEtat());
						}
						
						else if (event.type==sf::Event::KeyPressed && moteur.getEtat().getFin() == false){
							stateLayer.gestionCurseur(event, largeur_map_cases, longueur_map_cases, moteur.getEtat());
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

				sf::RenderWindow window(sf::VideoMode(400,400),"Map");
				StateLayer stateLayer(moteur.getEtat(), window);
				stateLayer.initSurfaces(moteur.getEtat());
				
				//----------------------------
				StateLayer* ptr_stateLayer=&stateLayer;
				moteur.getEtat().registerObserver(ptr_stateLayer);

				Moteur* ptr_moteur=&moteur;
				stateLayer.registerObserver(ptr_moteur);

				//------------------------
				
				//sf::RenderWindow window(sf::VideoMode(longueur_map_cases*stateLayer.getTilesets()[0]->getCellHeight(),
													  //largeur_map_cases*stateLayer.getTilesets()[0]->getCellWidth()),
													  //"Map");
				

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
						
							moteur.update();							
							
												
							if(moteur.verificationFinDeTour()){
								moteur.verificationDebutDeTour();
								StateEvent majDisponibilite(ALLCHANGED);
								moteur.getEtat().notifyObservers(majDisponibilite, moteur.getEtat());
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
						
							moteur.update();							
														
							if(moteur.verificationFinDeTour()){
								moteur.verificationDebutDeTour();
								StateEvent majDisponibilite(ALLCHANGED);
								moteur.getEtat().notifyObservers(majDisponibilite, moteur.getEtat());
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
						
							moteur.update();							
														
							if(moteur.verificationFinDeTour()){
								moteur.verificationDebutDeTour();
								StateEvent majDisponibilite(ALLCHANGED);
								moteur.getEtat().notifyObservers(majDisponibilite, moteur.getEtat());
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
			sf::RenderWindow window(sf::VideoMode(400,400),"Map");
			// -- Affichage de cet Etat --
			StateLayer layer(etat_initial, window);			
			// La variable layer.getTilesets()[0] est le tileset de la grille
			//sf::RenderWindow window(sf::VideoMode(	longueur_map_cases*layer.getTilesets()[0]->getCellHeight(),
													//largeur_map_cases*layer.getTilesets()[0]->getCellWidth()),
													//"Map");
			
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
