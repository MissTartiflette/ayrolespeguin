#include <SFML/Graphics.hpp>
#include "render.h"

using namespace render;

Layer::Layer (){
	
}

Layer::~Layer (){
	
}

Surface* Layer::getSurface (){
	return this->surface;
}

void Layer::setSurface (Surface* surface){
	this->surface=surface;
}

