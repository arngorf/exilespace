#ifndef TEXTUREUTILS_HPP
#define TEXTUREUTILS_HPP

#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>


namespace esGraphics {

// Error logger function for handling SDL errors.
void logSDLError(std::ostream &os, const std::string &msg);

// Texture loading wrapper function.
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);

// Texture rendering function for wrapping texture drawing.
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);

} // namespace esGraphics

#endif // TEXTUREUTILS_HPP