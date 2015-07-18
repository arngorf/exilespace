#include "Moveable.hpp"

namespace esEntity {

Moveable::Moveable(double xIn, double yIn){
    x = xIn;
    y = yIn;
    center.x = xIn;
    center.y = yIn;
    dx = 0;
    dy = 0;
    angle = 0;
    textureRect = new SDL_Rect;
}

Moveable::~Moveable() {
    delete textureRect;
}

void Moveable::update() {

    x += dx;
    y += dy;
    angle += da;

    center.x = x;
    center.y = y;

}

void Moveable::turnLeft() {
    da = -1;
}

void Moveable::turnRight() {
    da = 1;
}

void Moveable::stopTurn() {
    da = 0;
}

void Moveable::moveForward() {

}

void Moveable::stopForwardMovement() {

}

void Moveable::setTextureRectangle(double xIn, double yIn) {

    textureCenter.x = textureRect->w/2.0;
    textureCenter.y = textureRect->h/2.0;

    textureRect->x = xIn - textureRect->w/2.0;
    textureRect->y = yIn - textureRect->h/2.0;

}

} // namespace esEntity