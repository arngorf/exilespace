
#include "TestShip.hpp"

namespace esUnits {

TestShip::TestShip(double xIn, double yIn, esGraphics::Textures* texts) : Ship(xIn, yIn) {

    name = "Test Ship";
    textureRect->w = 64;
    textureRect->h = 64;
    texture = texts->dummyShip;

    angle = 0;
}

TestShip::~TestShip() {

}

std::string TestShip::getName() {
    return name;
}

void TestShip::draw(SDL_Renderer *ren) {

    SDL_RenderCopyEx(ren, texture, NULL, textureRect, angle, &textureCenter, SDL_FLIP_NONE);
}

} // namespace esUnits