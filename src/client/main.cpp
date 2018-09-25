#include <iostream>
#include <string.h>

// Les lignes suivantes ne servent qu'à vérifier que la compilation avec SFML fonctionne
#include <SFML/Graphics.hpp>

void testSFML() {
    sf::Texture texture;
}

// Fin test SFML

#include "state.h"

using namespace std;
using namespace state;

int main(int argc,char* argv[]) 
{
    Exemple exemple;
    exemple.setX(53);

    if (argc>1){
	if (strcmp(argv[1], "hello")==0){
	    cout << "Bonjour le monde !" << endl;
	}
	else{
	   exit(EXIT_FAILURE);
	}
    }

    return 0;
}