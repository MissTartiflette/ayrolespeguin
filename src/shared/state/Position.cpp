#include "state.h"

using namespace state;

int Position::getX(){
    return x;
}

int Position::getY(){
    return y;
}

void Position::setX(int newX){
    x = newX;
}

void Position::setY(int newY){
    y = newY;
}

bool Position::equals(Position& other){
	bool resultat;
	if (x == other.getX() && y == other.getY()){
		resultat = true;
	}
	else{
		resultat = false;
	}
	return resultat;
}
