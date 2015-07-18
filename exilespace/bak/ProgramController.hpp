

/*
 * Program Controller class
 * Singleton class
 *
 *
 *
 */

#ifndef PROGRAMCONTROLLER_HPP
#define PROGRAMCONTROLLER_HPP

#include <SDL.h>

#include "../esTools/Debug.hpp"

#include "GameSession.hpp"
#include "../esView/ActiveScreen.hpp"
#include "../esGraphics/Textures.hpp"
#include "../esTools/convolution_fftw.hpp"


namespace esControl {

class GameSession;

class ProgramController {
public:
    ProgramController();
    ~ProgramController();

    void start();

    SDL_Window* getWindow() {return win;};
    SDL_Renderer* getRenderer() {return ren;};
    esGraphics::Textures* getTextures() {return textures;};
    esView::ActiveScreen* getActiveScreen() {return as;};



    SDL_Window *win;
    SDL_Renderer *ren;
    esGraphics::Textures *textures;

private:

    esView::ActiveScreen *as;

    GameSession* gs;

    static const int SCREEN_WIDTH  = 960;
    static const int SCREEN_HEIGHT = 540;
};

} // namespace esControl

#endif // PROGRAMCONTROLLER_HPP