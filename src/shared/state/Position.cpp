#include "state.h"
#include <iostream>

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
