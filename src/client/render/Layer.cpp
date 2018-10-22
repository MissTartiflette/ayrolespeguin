#include <SFML/Graphics.hpp>

#include "render.h"
using namespace render;
#include <iostream>

Layer::Layer(){
	ptr_entity=0;
	
}

Layer::~Layer(){
	ptr_entity=0;
}

std::unique_ptr<MyEntity> Layer::getPtrEntity(){
	return std::move(ptr_entity);
}
