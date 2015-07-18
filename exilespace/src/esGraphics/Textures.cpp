
#include "Textures.hpp"

namespace esGraphics {

Textures::Textures(SDL_Renderer* ren) {
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    background = loadTexture("../resources/images/starfield.bmp", ren);
    dummyShip = loadTexture("../resources/images/ship.png", ren);
    mothership1 = loadTexture("../resources/images/mothership1.png", ren);
}

Textures::~Textures() {

}

} // namespace esGraphics