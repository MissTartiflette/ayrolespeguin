#include <SFML/Graphics.hpp>
#include <string>
#include "render.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>

using namespace render;
using namespace std;
using namespace state;

StateLayer::StateLayer(state::Etat& etat):etatLayer(etat){
}

std::vector<Surface> StateLayer::initSurface(){
	std::vector<Surface> tableSurface;
	TileSet tilesetGrille(GRILLETILESET);
	TileSet tilesetPerso(PERSONNAGETILESET);
	
	Surface surfGrille;
	Surface surfPersonnage;
	
	surfGrille.loadGrille(etatLayer,tilesetGrille.getImageFile(), sf::Vector2u(tilesetGrille.getCellWidth(), tilesetGrille.getCellHeight()), etatLayer.getGrille().size(), etatLayer.getGrille()[0].size());

	surfPersonnage.loadPersonnage(etatLayer,tilesetPerso.getImageFile(), sf::Vector2u(tilesetPerso.getCellWidth(), tilesetPerso.getCellHeight()), etatLayer.getPersonnages().size(), 1);

	tableSurface.push_back(surfGrille);
	tableSurface.push_back(surfPersonnage);

	return tableSurface;
}


