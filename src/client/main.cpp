#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>

// Les lignes suivantes ne servent qu'à vérifier que la compilation avec SFML fonctionne
#include <SFML/Graphics.hpp>


void testSFML() {
    sf::Texture texture;
}

// Fin test SFML

#include "state.h"
#include "render.h"
using namespace std;
using namespace state;
using namespace render;

int main(int argc,char* argv[]){

	testSFML();
	
	if (argc>1){
		/*	hello : Affichage simple */
		if(strcmp(argv[1],"hello")==0){
			cout<<"Bonjour tout le monde"<<endl;
		}
		
		/*	render : Affichage du rendu de la map */
		else if(strcmp(argv[1],"render")==0){		
			cout<<"Affichage map"<<endl;
			
			std::string chemin_fichier_map_txt = "res/map2.txt";
			std::string chemin_tileset_map = "res/tileset.png";
			std::string contenu, ligne, code_tuile;
			const size_t longueur_tuile = 16, largeur_tuile = 16;
			const size_t longueur_map_cases = 25, largeur_map_cases = 25;
			int map_tuiles_code[longueur_map_cases*largeur_map_cases];			
			    		
    		// Fichier
    		ifstream fichier(chemin_fichier_map_txt, ios::in);
    		if (fichier){
    			while (getline(fichier,ligne)){
					ligne = ligne + ",";
					contenu = contenu + ligne;
				}
				fichier.close();
    		}
    		else {return -1;}    		
    		    		
    		// Conversion des codes des tuiles en int
    		std::stringstream contenuStream(contenu);
    		size_t i = 0;
    		    		
    		while(std::getline(contenuStream, code_tuile, ',')){
    			map_tuiles_code[i] = std::stoi(code_tuile);
    			i++;
    		}
    								
			// Fenetre
    		sf::RenderWindow window(sf::VideoMode(longueur_tuile*longueur_map_cases, largeur_tuile*largeur_map_cases), "Tilemap");
			Surface map;
						
			if (!map.load(chemin_tileset_map, sf::Vector2u(longueur_tuile, largeur_tuile), map_tuiles_code, longueur_map_cases, largeur_map_cases))
				return -1;

			while (window.isOpen())
			{
				// Gestion des evenements
				sf::Event event;
				while (window.pollEvent(event))	{
				    if(event.type == sf::Event::Closed)
				        window.close();
				}

				window.clear();
				window.draw(map);
				window.display();
			}
		}

		/*	state : Tests unitaires*/
		else if(strcmp(argv[1],"state")==0){
					
			Personnage perso = Personnage(ARCHER);
			
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
