#include <SFML/Graphics.hpp>
#include <string>
#include "render.h"

using namespace render;
/**
void StateLayer::initSurface (const std::string& tileset, unsigned int width, unsigned int height, const int* tiles, sf::Vector2u tileSize, Surface surface){
	surface.loadTexture(tileset);
	// on redimensionne le tableau de vertex pour qu'il puisse contenir tout le niveau
     surface.initQuads(width, height);

    // on remplit le tableau de vertex, avec un quad par tuile
     for (unsigned int i = 0; i < width; ++i){
     	for (unsigned int j = 0; j < height; ++j){
            // on récupère le numéro de tuile courant
            int tileNumber = tiles[i + j * width];

            // on en déduit sa position dans la texture du tileset
            int tu = tileNumber % (surface.getTexture().getSize().x / tileSize.x);
            int tv = tileNumber / (surface.getTexture().getSize().x / tileSize.x);

            // on récupère un pointeur vers le quad à définir dans le tableau de vertex
            sf::Vertex* quad = &surface.getQuads()[(i + j * width) * 4];

			// on définit ses quatre coins
			surface.setQuadsLocation(quad, tileSize, i, j);

            // on définit ses quatre coordonnées de texture
			surface.setQuadsTexture(quad, tileSize, tu, tv);
                
		}
	}
}

*/

