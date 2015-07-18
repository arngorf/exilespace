
#include "Mothership.hpp"

namespace esUnits {

Mothership::Mothership(double xIn, double yIn, esGraphics::Textures* texts) : Ship(xIn, yIn) {

    name = "Mothership";
    textureRect->w = 256;
    textureRect->h = 256;
    texture = texts->mothership1;

    mainEngine = new esSubsystems::StandardEngine();

}

Mothership::~Mothership() {
    delete mainEngine;
}

std::string Mothership::getName() {
    return name;
}

void Mothership::moveForward() {
    if (mainEngine != nullptr) {
        mainEngine->activate();
    }
}

void Mothership::stopForwardMovement() {
    if (mainEngine != nullptr) {
        mainEngine->deactivate();
    }
}

void Mothership::update() {

    double* xPtr = &dx;
    double* yPtr = &dy;

    if (mainEngine != nullptr) {
        mainEngine->setNewSpeed(xPtr, yPtr, angle, 1.0, 100);
    }

    x += dx;
    y += dy;
    angle = modAngle(angle + da);

    center.x = x;
    center.y = y;


}


void Mothership::draw(SDL_Renderer *ren) {
    SDL_RenderCopyEx(ren, texture, NULL, textureRect, angle, &textureCenter, SDL_FLIP_NONE);
}

} // namespace esUnits