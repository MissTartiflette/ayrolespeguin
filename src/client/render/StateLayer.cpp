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

	/*TileSet tilesetInfos(INFOSTILESET);
	std::unique_ptr<TileSet> ptr_tilesetInfos (new TileSet(tilesetInfos));
	tilesets.push_back(move(ptr_tilesetInfos));*/
}

void StateLayer::initSurfaces(state::Etat& etat){	
	Surface surfGrille;
	Surface surfPersonnage;
	Surface surfCurseur;
	//Surface surfInfos;

	surfGrille.loadGrille(etat, tilesets[0]->getTexture(), sf::Vector2u(tilesets[0]->getCellWidth(), tilesets[0]->getCellHeight()), etat.getGrille().size(), etat.getGrille()[0].size());

	surfPersonnage.loadPersonnage(etat, tilesets[1]->getTexture(), sf::Vector2u(tilesets[1]->getCellWidth(), tilesets[1]->getCellHeight()), etat.getPersonnages().size(), 1);

	surfCurseur.loadCurseur(etat, tilesets[2]->getTexture(), sf::Vector2u(tilesets[2]->getCellWidth(), tilesets[2]->getCellHeight()), 1, 1);
	
	std::unique_ptr<Surface> ptr_surfGrille (new Surface(surfGrille));
	std::unique_ptr<Surface> ptr_surfPersonnage (new Surface(surfPersonnage));
	std::unique_ptr<Surface> ptr_surfCurseur (new Surface(surfCurseur));
	//std::unique_ptr<Surface> ptr_surfInfos (new Surface(surfInfos));
	
	if(surfaces.size()!=0){
		while(surfaces.size()!=0){
			surfaces.pop_back();
		}
	}
	
	surfaces.push_back(move(ptr_surfGrille));
	surfaces.push_back(move(ptr_surfPersonnage));
	surfaces.push_back(move(ptr_surfCurseur));
	//surfaces.push_back(move(ptr_surfInfos));
	
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
	if (e.stateEventID == ALLCHANGED){
		initSurfaces(etat);
		draw(window);
	}
	if (e.stateEventID == TEXTECHANGED){
		writeTexteAction(e.texte, window);
	}
}

void StateLayer::draw (sf::RenderWindow& window){
	window.clear();
	window.draw(*surfaces[0]);	// Dessin de la grille				
	window.draw(*surfaces[1]);	// Dessin des personnages
	window.draw(*surfaces[2]);	// Dessin du curseur
	//window.draw(*surfaces[3]);	// Dessin des infos
	
	// create a quad
	sf::VertexArray quad(sf::Quads, 4);

	// Rectangle degrade en (0,400) et de taille 400x200
	quad[0].position = sf::Vector2f(0.f, 400.f);
	quad[1].position = sf::Vector2f(400.f, 400.f);
	quad[2].position = sf::Vector2f(400.f, 600.f);
	quad[3].position = sf::Vector2f(0.f, 600.f);
	quad[0].color = sf::Color::Blue;
	quad[1].color = sf::Color::Red;
	quad[2].color = sf::Color::Red;
	quad[3].color = sf::Color::Blue;
	
	sf::RectangleShape rectangle(sf::Vector2f(390.f, 30.f));
	rectangle.setPosition(5.f, 405.f);
	rectangle.setFillColor(sf::Color::Black);
	
	window.draw(quad);
	window.draw(rectangle);
	/*
	sf::Font font;
	font.loadFromFile("res/8-BIT-WONDER.TTF");
	sf::Text text;
	text.setFont(font);
	text.setString("Fire Emblem Unofficial");
	text.setCharacterSize(10);
	text.setFillColor(sf::Color::White);
	text.setPosition(10.f, 400.f);
	
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width/2.0f,
		           textRect.top  + textRect.height/2.0f);
	text.setPosition(sf::Vector2f(400/2.0f, 420));
	window.draw(text);*/
	
	window.display();
}

void StateLayer::writeTexteAction(const std::string chaine, sf::RenderWindow& window){
	sf::RectangleShape rectangle(sf::Vector2f(390.f, 30.f));
	rectangle.setPosition(5.f, 405.f);
	rectangle.setFillColor(sf::Color::Black);
	
	window.draw(rectangle);
	
	sf::Font font;
	font.loadFromFile("res/8-BIT-WONDER.TTF");
	sf::Text text;
	text.setFont(font);
	text.setString(chaine);
	text.setCharacterSize(10);
	text.setFillColor(sf::Color::White);
	text.setPosition(10.f, 400.f);
	
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width/2.0f,
		           textRect.top  + textRect.height/2.0f);
	text.setPosition(sf::Vector2f(400/2.0f, 420));
	window.draw(text);
	window.display();
}


