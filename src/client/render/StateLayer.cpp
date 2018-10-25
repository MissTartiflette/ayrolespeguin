#include <SFML/Graphics.hpp>
#include <string>
#include "render.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace render;
using namespace std;

StateLayer::StateLayer(state::Etat& etat):etatLayer(etat){
	  
}

std::vector<Surface> StateLayer::initSurface(unsigned int longueur, unsigned int largeur){
	std::vector<Surface> tableSurface;

	TileSet tilesetGrille(GRILLETILESET);
	TileSet tilesetPerso(PERSONNAGETILESET);

	Surface surfGrille;
	Surface surfPersonnage;

	surfGrille.loadGrille(etatLayer,tilesetGrille.getImageFile(), sf::Vector2u(tilesetGrille.getCellWidth(), tilesetGrille.getCellHeight()), largeur, longueur);
	surfPersonnage.loadPersonnage(etatLayer,tilesetPerso.getImageFile(), sf::Vector2u(tilesetPerso.getCellWidth(), tilesetPerso.getCellHeight()), largeur, longueur);

	tableSurface.push_back(surfGrille);
	tableSurface.push_back(surfPersonnage);

	return tableSurface;
}


