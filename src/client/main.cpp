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
using namespace std;
using namespace state;
using namespace render;
using namespace engine;

int main(int argc,char* argv[]){

	testSFML();
		
	if (argc>1){
		/*	hello : Affichage simple */
		if(strcmp(argv[1],"hello")==0){
			cout<<"Bonjour tout le monde"<<endl;
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
				StateLayer stateLayer(moteur.getEtat());
				stateLayer.initSurfaces(moteur.getEtat());
				
				//----------------------------
				StateLayer* ptr_stateLayer=&stateLayer;
				moteur.getEtat().registerObserver(ptr_stateLayer);

				//------------------------
				
				sf::RenderWindow window(sf::VideoMode(longueur_map_cases*stateLayer.getTilesets()[0]->getCellHeight(),
													  largeur_map_cases*stateLayer.getTilesets()[0]->getCellWidth()),
													  "Map");
													  
				int i=0;
				while (window.isOpen()){
					sf::Event event;
					while (window.pollEvent(event)){
						if (event.type == sf::Event::Closed){
							window.close();
						}
						// Si on appuie sur une touche et que la partie n'est pas terminee
						else if(event.type==sf::Event::KeyPressed && !moteur.getEtat().getFin()){
							moteur.update(window);
							
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
														
							if(moteur.verificationFinDeTour()){
								moteur.verificationDebutDeTour();
								StateEvent majDisponibilite(ALLCHANGED);
								moteur.getEtat().notifyObservers(majDisponibilite, moteur.getEtat(), window);
							}
						}						
					}
					
					
						
					
					
					if(i==0){
						i=1;
						stateLayer.draw(window);
						cout << "Tour " << moteur.getEtat().getTour() << endl;
												
						// Deplacement chevalier bleu
						Position destination1(3,22);
						Deplacement deplacement1(*moteur.getEtat().getPersonnages()[2], destination1,true);
						unique_ptr<Commande> ptr_deplacement1 (new Deplacement(deplacement1));
						moteur.addCommande(0, move(ptr_deplacement1));						
						
						// Attaque chevalier bleu contre archer rouge
						Attaque attaque1(*moteur.getEtat().getPersonnages()[2], *moteur.getEtat().getPersonnages()[4],true);
						unique_ptr<Commande> ptr_attaque1 (new Attaque(attaque1));
						moteur.addCommande(1, move(ptr_attaque1));
						
						// Pas d'actions pour les autres personnages bleus
						
						// Deplacement chevalier bleu
						Position destination2(1,20);
						Deplacement deplacement2(*moteur.getEtat().getPersonnages()[0], destination2,true);
						unique_ptr<Commande> ptr_deplacement2 (new Deplacement(deplacement2));
						moteur.addCommande(2, move(ptr_deplacement2));
												
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
						
						FinActions finattaque0(*moteur.getEtat().getPersonnages()[0],true);
						unique_ptr<Commande> ptr_finactions0 (new FinActions(finattaque0));
						moteur.addCommande(6, move(ptr_finactions0));
					}
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
