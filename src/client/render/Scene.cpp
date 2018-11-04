#include "render.h"

using namespace render;
using namespace state;

  
Scene::Scene (state::Etat& etat) : etat(etat), stateLayer(etat){
	stateLayer.initSurfaces();
	width=0;
	height=0;
}

size_t Scene::getWidth (){
	return width;
}

size_t Scene::getHeight (){
	return height;
}

void Scene::stateChanged (const state::StateEvent& e, state::Etat& etat){
		stateLayer.initSurfaces();
}

void Scene::draw (sf::RenderWindow& window){
	window.clear();
	window.draw(*stateLayer.getSurfaces()[0]);	// Dessin de la grille				
	window.draw(*stateLayer.getSurfaces()[1]);	// Dessin des personnages
	window.display();
}

StateLayer& Scene::getStateLayer(){
	StateLayer& refStateLayer=stateLayer;
	return refStateLayer;
}
