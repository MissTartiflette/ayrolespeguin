#include <SFML/Graphics.hpp>
#include <string>
#include "render.h"

using namespace render;


bool Surface::loadGrille(state::Etat& etatLayer, const std::string& tileset, sf::Vector2u tileSize, unsigned int width, unsigned int height){
    
        // on charge la texture du tileset
        if (!texture.loadFromFile(tileset)){
            return false;
		}
      	// on redimensionne le tableau de vertex pour qu'il puisse contenir tout le niveau
	   	quads.setPrimitiveType(sf::Quads);
       	quads.resize(width * height * 4);

        // on remplit le tableau de vertex, avec un quad par tuile
        for (unsigned int i = 0; i < width; ++i){
            for (unsigned int j = 0; j < height; ++j){

            	// on récupère le numéro de tuile courant
				int tileNumber=etatLayer.getGrille()[i][j]->getCodeTuile();
				
                // on en déduit sa position dans la texture du tileset
                int tu = tileNumber % (texture.getSize().x / tileSize.x);
                int tv = tileNumber / (texture.getSize().x / tileSize.x);

                // on récupère un pointeur vers le quad à définir dans le tableau de vertex
                sf::Vertex* quad = &quads[(i + j * width) * 4];
				
				// on définit ses quatre coins
				quad[0].position = sf::Vector2f(etatLayer.getGrille()[i][j]->getPosition().getX() * tileSize.x, etatLayer.getGrille()[i][j]->getPosition().getY() * tileSize.y);
				quad[1].position = sf::Vector2f((etatLayer.getGrille()[i][j]->getPosition().getX()+ 1) * tileSize.x, etatLayer.getGrille()[i][j]->getPosition().getY() * tileSize.y);
				quad[2].position = sf::Vector2f((etatLayer.getGrille()[i][j]->getPosition().getX() + 1) * tileSize.x, (etatLayer.getGrille()[i][j]->getPosition().getY() + 1) * tileSize.y);
				quad[3].position = sf::Vector2f(etatLayer.getGrille()[i][j]->getPosition().getX() * tileSize.x, (etatLayer.getGrille()[i][j]->getPosition().getY() + 1) * tileSize.y);
				
// on définit ses quatre coordonnées de texture
				quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
				quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
				quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
				quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
							
           }
		}

		return true;				
}


bool Surface::loadPersonnage(state::Etat& etatLayer, const std::string& tileset, sf::Vector2u tileSize, unsigned int width, unsigned int height){
    
        // on charge la texture du tileset
        if (!texture.loadFromFile(tileset)){
            return false;
		}
      	// on redimensionne le tableau de vertex pour qu'il puisse contenir tout le niveau
	   	quads.setPrimitiveType(sf::Quads);
       	quads.resize(width * height * 4);
        // on remplit le tableau de vertex, avec un quad par tuile
        for (unsigned int i = 0; i < width; ++i){
           for (unsigned int j = 0; j < height; ++j){

            	// on récupère le numéro de tuile courant
				int tileNumber=etatLayer.getPersonnages()[i]->getCodeTuile();
				
                // on en déduit sa position dans la texture du tileset
                int tu = tileNumber % (texture.getSize().x / tileSize.x);
                int tv = tileNumber / (texture.getSize().x / tileSize.x);

                // on récupère un pointeur vers le quad à définir dans le tableau de vertex
                sf::Vertex* quad = &quads[(i + j * width) * 4];
				
				// on définit ses quatre coins
				quad[0].position = sf::Vector2f(etatLayer.getPersonnages()[i]->getPosition().getX() * tileSize.x, etatLayer.getPersonnages()[i]->getPosition().getY() * tileSize.y);
				quad[1].position = sf::Vector2f((etatLayer.getPersonnages()[i]->getPosition().getX()+ 1) * tileSize.x, etatLayer.getPersonnages()[i]->getPosition().getY() * tileSize.y);
				quad[2].position = sf::Vector2f((etatLayer.getPersonnages()[i]->getPosition().getX() + 1) * tileSize.x, (etatLayer.getPersonnages()[i]->getPosition().getY() + 1) * tileSize.y);
				quad[3].position = sf::Vector2f(etatLayer.getPersonnages()[i]->getPosition().getX() * tileSize.x, (etatLayer.getPersonnages()[i]->getPosition().getY() + 1) * tileSize.y);
				
// on définit ses quatre coordonnées de texture
				quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
				quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
				quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
				quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
							
          } 
		}

		return true;
}


void Surface::draw(sf::RenderTarget& target, sf::RenderStates states) const {

        // on applique la transformation
        states.transform *= getTransform();

        // on applique la texture du tileset
        states.texture = &texture;

        // et on dessine enfin le tableau de vertex
        target.draw(quads, states);
}

