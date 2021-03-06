#include "state.h"

using namespace state;

Position::Position(){
	this->x=0;
	this->y=0;
}

Position::Position(int x, int y){
	this->x=x;
	this->y=y;
}

int Position::getX(){
    return this->x;
}

int Position::getY(){
    return this->y;
}

void Position::setX(int newX){
    this->x = newX;
}

void Position::setY(int newY){
    this->y = newY;
}

bool Position::equals(Position& other){
	bool resultat;
	if (this->x == other.getX() && this->y == other.getY()){
		resultat = true;
	}
	else{
		resultat = false;
	}
	return resultat;
}

int Position::distance(Position& other){
	return abs(x-other.getX())+abs(y-other.getY());
}

