#include <iostream>
#include <string.h>

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
		if(strcmp(argv[1],"hello")==0){
			cout<<"Bonjour tout le monde"<<endl;
		}
		else if(strcmp(argv[1],"render")==0){
			cout<<"Affichage map"<<endl;
			
    		sf::RenderWindow window(sf::VideoMode(400, 400), "Tilemap");

    // on définit le niveau à l'aide de numéro de tuiles
    const int level[] =
    {
        0,0,126,126,126,60,32,15,82,32,32,32,16,82,60,6,59,32,82,117,85,85,85,85,117,
		0,56,57,126,6,16,60,58,82,82,58,32,60,6,16,16,6,82,82,84,80,61,61,81,84,
		16,1,56,57,1,0,60,6,60,58,6,1,1,16,32,16,59,1,58,84,60,60,61,61,84,
		6,32,1,34,59,60,1,36,60,1,16,60,59,1,16,1,60,16,32,84,16,6,16,61,84,
		1,6,59,35,16,1,60,58,6,164,1,60,6,60,59,16,59,59,58,60,16,59,16,80,84,
		1,16,6,33,60,60,1,6,32,60,59,1,1,32,58,59,16,16,60,16,6,85,85,85,117,
		6,59,1,56,57,1,60,60,6,59,1,59,6,16,6,1,1,6,16,59,32,58,15,32,58,
		15,16,16,60,56,57,3,4,59,16,59,16,82,59,6,59,1,16,60,1,59,60,6,58,82,
		60,16,59,0,59,56,1005,1008,4,59,1,13,14,59,1,1,16,58,59,59,16,1,6,1,126,
		60,6,16,1,16,59,59,59,35,60,59,1,6,1,59,16,6,59,16,1,16,60,58,126,126,
		82,16,16,59,16,59,60,59,33,58,1,1,6,60,1,59,6,59,59,6,16,1,59,126,126,
		1,16,6,6,1,16,32,59,56,1001,1000,1002,57,59,60,6,1,1,1,1,58,59,1,60,126,
		126,32,60,32,81,82,60,1,1,16,1,82,34,15,1,16,1,1,1,15,164,15,60,32,126,
		126,1,6,1,82,16,1,59,1,60,1,16,56,1002,1000,1001,57,1,58,60,1,16,60,60,126,
		126,126,60,1,59,59,6,1,6,1,1,16,60,6,1,58,33,60,60,1,1,16,16,60,58,
		126,126,58,16,1,16,59,16,6,59,6,6,59,6,59,60,35,60,59,59,16,1,60,60,60,
		126,60,6,59,59,16,59,32,16,59,59,59,13,14,16,6,56,57,5,4,16,0,1,6,60,
		58,32,6,16,60,60,6,6,59,6,1,59,32,6,1,60,16,56,2,56,57,1,1,6,32,
		32,15,32,58,82,59,1,1,6,59,60,60,60,1,6,59,16,16,6,59,56,57,16,1,6,
		117,85,85,85,1,60,6,6,1,60,32,82,16,1,16,16,82,6,59,16,6,33,6,60,59,
		84,80,6,6,59,59,15,6,60,59,1,16,60,60,1,81,16,82,16,60,16,35,16,1,16,
		84,61,6,60,6,84,82,6,16,6,60,1,60,6,1,1,59,36,59,1,6,34,16,15,16,
		84,61,61,59,59,84,82,1,1,60,15,60,59,1,6,32,6,60,6,0,16,56,57,1,60,
		84,81,61,61,80,84,15,32,60,6,60,60,60,60,15,58,58,32,16,6,6,126,56,57,6,
		117,85,85,85,85,117,58,82,6,60,6,32,59,15,82,15,15,32,15,59,126,126,126,56,57,
    };

    // on crée la tilemap avec le niveau précédemment défini
    MyEntity map;
    if (!map.load("/home/chloe/Documents/ayrolespeguin/res/tileset.png", sf::Vector2u(16, 16), level, 25, 25))
        return -1;

    // on fait tourner la boucle principale
    while (window.isOpen())
    {
        // on gère les évènements
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }

        // on dessine le niveau
        window.clear();
        window.draw(map);
        window.display();
    }
		}


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
