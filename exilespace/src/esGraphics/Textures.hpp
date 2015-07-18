/*
 * Test ship class
 *
 */


#ifndef TEXTURES_HPP
#define TEXTURES_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "TextureUtils.hpp"

namespace esGraphics {

class Textures {
public:
    Textures(SDL_Renderer* ren);
    ~Textures();

    // TEXTURES

    // SHIPS
    SDL_Texture* background;
    SDL_Texture* dummyShip;
    SDL_Texture* mothership1;

};

} // namespace esGraphics

#endif // TEXTURES_HPP