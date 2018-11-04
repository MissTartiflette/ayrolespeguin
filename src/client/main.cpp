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
			cout<<"Moteur du jeu"<<endl;
			//----------------------------
			unsigned int longueur_map_cases = 25, largeur_map_cases = 25;
			std::string chemin_fichier_map_txt = "res/map1.txt";
			
			Correspondances tab_corres = Correspondances();
			
			//-----------------------------
			Moteur moteur;
			if(moteur.getEtat().initGrille(chemin_fichier_map_txt, longueur_map_cases, largeur_map_cases, tab_corres)){
				moteur.getEtat().initPersonnages(tab_corres);
				Scene scene(moteur.getEtat());
				//----------------------------
			
				//Observable observable;
			
				//Scene* ptr_scene=&scene;
				//observable.registerObserver(ptr_scene);

				//------------------------
				
				sf::RenderWindow window(sf::VideoMode(longueur_map_cases*scene.getStateLayer().getTilesets()[0]->getCellHeight(),
													  largeur_map_cases*scene.getStateLayer().getTilesets()[0]->getCellWidth()),
													  "Map");
				int i=0;
				while (window.isOpen()){
					sf::Event event;
					while (window.pollEvent(event)){
						if (event.type == sf::Event::Closed){
							window.close();
						}
						else if(event.type==sf::Event::KeyPressed){
							moteur.update(window);

							Attaque attaque(*moteur.getEtat().getPersonnages()[2], *moteur.getEtat().getPersonnages()[4]);
							unique_ptr<Commande> ptr_attaque (new Attaque(attaque));
							moteur.addCommande(0, move(ptr_attaque));
						}
						
					}
					if(i==0){
						i=1;
						scene.draw(window);
						
						Position destination1(3,22);
						Deplacement deplacement1(*moteur.getEtat().getPersonnages()[2], destination1);
						unique_ptr<Commande> ptr_deplacement1 (new Deplacement(deplacement1));
						moteur.addCommande(0, move(ptr_deplacement1));

						Position destination2(1,22);
						Deplacement deplacement2(*moteur.getEtat().getPersonnages()[0], destination2);
						unique_ptr<Commande> ptr_deplacement2 (new Deplacement(deplacement2));
						moteur.addCommande(1, move(ptr_deplacement2));

						Attaque attaque(*moteur.getEtat().getPersonnages()[2], *moteur.getEtat().getPersonnages()[3]);
						std::unique_ptr<Commande> ptr_attaque (new Attaque(attaque));
						moteur.addCommande(2, move(ptr_attaque));
					}
				
					
					
				}

			
			}
				

		}


		/*	render : Affichage du rendu de la map */
		else if(strcmp(argv[1],"render")==0){
		
			cout<<"Affichage d'un Etat"<<endl;
			
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
			
			layer.initSurfaces();
	
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
