#include "ProgramController.hpp"

namespace esControl {

ProgramController* ProgramController::_instance = nullptr;

ProgramController* ProgramController::Instance() {

esTools::Debug("I: DEBUG: ProgramController::Instance: CALLED");

    if (_instance == nullptr) {
        _instance = new ProgramController;
    }
    return _instance;

    esTools::Debug("I: DEBUG: ProgramController::Instance: FINISHED");
}

ProgramController::ProgramController() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cout << "E: DEBUG: SDL_Init ERROR: " << SDL_GetError() << std::endl;
        exit(1);
    } else {
        std::cout << "I: DEBUG: SDL_Init SUCCESS:" << std::endl;
    }

    win = SDL_CreateWindow("Exile Space", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (win == nullptr){
        std::cout << "E: DEBUG: SDL_CreateWindow ERROR:" << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    } else {
        std::cout << "I: DEBUG: SDL_CreateWindow SUCCESS:" << std::endl;
    }

    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr){
        SDL_DestroyWindow(win);
        std::cout << "E: DEBUG: SDL_CreateRenderer ERROR:" << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    } else {
        std::cout << "I: DEBUG: SDL_CreateRenderer SUCCESS:" << std::endl;
    }

    as = new esView::ActiveScreen(-50,-70,SCREEN_WIDTH,SCREEN_HEIGHT);
    textures = new esGraphics::Textures(ren);

}

void ProgramController::start() {

    gs = new GameSession(_instance);

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