#include "TextureUtils.hpp"

namespace esGraphics {

void logSDLError(std::ostream &os, const std::string &msg){
    os << msg << " error: " << SDL_GetError() << std::endl;
}

SDL_Texture* loadTexture(const std::string &file, SDL_Renderer* ren){
    // Initialize to nullptr to avoid dangling pointer issues
    SDL_Texture *texture = nullptr;
    // Load the image
    SDL_Surface *loadedImage = IMG_Load(file.c_str()); //SDL_LoadBMP(file.c_str());
    // If the loading went ok, convert to texture and return the texture
    if (loadedImage != nullptr){
        texture = SDL_CreateTextureFromSurface(ren, loadedImage);
        SDL_FreeSurface(loadedImage);
        //Make sure converting went ok too
        if (texture == nullptr){
            logSDLError(std::cout, "CreateTextureFromSurface");
        }
    }
    else {
        logSDLError(std::cout, "LoadBMP");
    }
    return texture;
}

} // namespace esGraphics