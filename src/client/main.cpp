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
    //Exemple exemple;
    //exemple.setX(53);

    Position testpos;
    testpos.setX(1);
    testpos.setY(2);

    cout << testpos.getX() << " et "<< testpos.getY() << endl;

    return 0;
}
