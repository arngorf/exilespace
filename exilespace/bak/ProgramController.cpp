#include "ProgramController.hpp"

namespace esControl {

ProgramController::ProgramController() {

    esTools::Debug("I: DEBUG: ProgramController::ProgramController: CALLED");

    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        exit(1);
    } else {
        std::cout << "It's working Jim, it's working!" << std::endl;
    }

    win = SDL_CreateWindow("Exile Space", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (win == nullptr){
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }

    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr){
        SDL_DestroyWindow(win);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }



    as = new esView::ActiveScreen(-50,-70,SCREEN_WIDTH,SCREEN_HEIGHT);
    textures = new esGraphics::Textures(ren);


    // THIS SHOULD NOT BE HERE
    /*std::string imagePath = "../resources/images/ship.bmp";
    SDL_Surface *bmp = SDL_LoadBMP(imagePath.c_str());
    if (bmp == nullptr){
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
    SDL_FreeSurface(bmp);
    if (tex == nullptr){
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }

    SDL_Rect texr; texr.x = 10; texr.y = 10; texr.w = 64; texr.h = 64;


    //A sleepy rendering loop, wait for 3 seconds and render and present the screen each time
    for (int i = 0; i < 100; ++i){
        //First clear the renderer
        texr.x += 1;
        texr.y += 1;
        SDL_RenderClear(ren);
        //Draw the texture
        SDL_RenderCopy(ren, tex, NULL, &texr);
        //Update the screen

    }

    SDL_DestroyTexture(tex);*/

    // STOP OF WHAT SHOULD NOT BE HERE

    gs = new GameSession(this);
    esTools::Debug("I: DEBUG: ProgramController::ProgramController: FINISHED");
}

void ProgramController::start() {
    gs->start();
}

ProgramController::~ProgramController() {
    esTools::Debug("I: DEBUG: ProgramController::~ProgramController: CALLED");

    delete textures;

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    esTools::Debug("I: DEBUG: ProgramController::~ProgramController: FINISHED");

}

} // namespace esControl