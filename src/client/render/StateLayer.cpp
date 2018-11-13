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


StateLayer::StateLayer(state::Etat& etat){
	
	TileSet tilesetGrille(GRILLETILESET);
	std::unique_ptr<TileSet> ptr_tilesetGrille (new TileSet(tilesetGrille));
	tilesets.push_back(move(ptr_tilesetGrille));
	
	TileSet tilesetPersonnages(PERSONNAGETILESET);
	std::unique_ptr<TileSet> ptr_tilesetPersonnages (new TileSet(tilesetPersonnages));
	tilesets.push_back(move(ptr_tilesetPersonnages));
	
	TileSet tilesetCurseur(CURSEURTILESET);
	std::unique_ptr<TileSet> ptr_tilesetCurseur (new TileSet(tilesetCurseur));
	tilesets.push_back(move(ptr_tilesetCurseur));

	// tilesetInfos(INFOSTILESET);
	//std::unique_ptr<TileSet> ptr_tilesetInfos (new TileSet(tilesetInfos));
	//tilesets.push_back(move(ptr_tilesetInfos));
	
}

void StateLayer::initSurfaces(state::Etat& etat){	
	Surface surfGrille;
	Surface surfPersonnage;
	Surface surfCurseur;

	surfGrille.loadGrille(etat, tilesets[0]->getImageFile(), sf::Vector2u(tilesets[0]->getCellWidth(), tilesets[0]->getCellHeight()), etat.getGrille().size(), etat.getGrille()[0].size());

	surfPersonnage.loadPersonnage(etat, tilesets[1]->getImageFile(), sf::Vector2u(tilesets[1]->getCellWidth(), tilesets[1]->getCellHeight()), etat.getPersonnages().size(), 1);

	surfCurseur.loadCurseur(etat, tilesets[2]->getImageFile(), sf::Vector2u(tilesets[2]->getCellWidth(), tilesets[2]->getCellHeight()), 1, 1);
	
	std::unique_ptr<Surface> ptr_surfGrille (new Surface(surfGrille));
	std::unique_ptr<Surface> ptr_surfPersonnage (new Surface(surfPersonnage));
	std::unique_ptr<Surface> ptr_surfCurseur (new Surface(surfCurseur));
	
	if(surfaces.size()!=0){
		while(surfaces.size()!=0){
			surfaces.pop_back();
		}
	}
	
	surfaces.push_back(move(ptr_surfGrille));
	surfaces.push_back(move(ptr_surfPersonnage));
	surfaces.push_back(move(ptr_surfCurseur));
	
}

std::vector<std::unique_ptr<TileSet>>& StateLayer::getTilesets (){
	std::vector<std::unique_ptr<TileSet>>& ref_tilesets = tilesets;
	return ref_tilesets;
}

std::vector<std::unique_ptr<Surface>>& StateLayer::getSurfaces (){
	std::vector<std::unique_ptr<Surface>>& ref_surfaces = surfaces;
	return ref_surfaces;
}

void StateLayer::stateChanged (const state::StateEvent& e, state::Etat& etat, sf::RenderWindow& window){
	initSurfaces(etat);
	draw(window);
}

void StateLayer::draw (sf::RenderWindow& window){
	window.clear();
	window.draw(*surfaces[0]);	// Dessin de la grille				
	window.draw(*surfaces[1]);	// Dessin des personnages
	window.draw(*surfaces[2]);	// Dessin du curseur
	window.display();
}

