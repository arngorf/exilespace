#include "GameSession.hpp"

namespace esControl {

GameSession::GameSession(ProgramController* controllerIn) {

    esTools::Debug("I: DEBUG: GameSession::GameSession: CALLED");

    controller = controllerIn;

    as = nullptr;
    ren = nullptr;

    while (controller->getTextures() == nullptr) {
        std::cout << "W: DEBUG: ProgramController::start: Textures not loaded yet ..." << std::endl;
    }


    board = new Gameboard(controller->getTextures());


    board->placeStartLocation(0,0);
    showSectionLines = true;

    esTools::Debug("I: DEBUG: GameSession::GameSession: FINISHED");

}
GameSession::~GameSession() {
    esTools::Debug("I: DEBUG: GameSession::~GameSession: CALLED");

    delete board;

    esTools::Debug("I: DEBUG: GameSession::~GameSession: FINISHED");
}

void GameSession::start() {
    esTools::Debug("I: DEBUG: GameSession::start: CALLED");

    as = controller->getActiveScreen();
    ren = controller->getRenderer();

    introScreenMainloop();

    esTools::Debug("I: DEBUG: GameSession::start: FINISHED");
}

void GameSession::introScreenMainloop() {
    esTools::Debug("I: DEBUG: GameSession::introScreenMainloop: CALLED");

    state = INTRO;

    while (false) {
        // This should show the intro screen
    }

    gameMainloop();

    esTools::Debug("I: DEBUG: GameSession::introScreenMainloop: FINISHED");
}

void GameSession::gameMainloop() {

    esTools::Debug("I: DEBUG: GameSession::gameMainloop: CALLED");

    state = PLAYING;

    while (!quit) {
        timestepInput();
        placeActiveScreen();
        timestepTimers();
        timestepEntities();
        board->transferEntities();
        timestepTriggers();
        timestepAI();

        timestepDraw();
        // Present to screen
        SDL_RenderPresent(ren);

        // Absolute delay is bad. This needs to be based on the time remaining until next frame assuming fps 60.
        SDL_Delay(10);

    }

    esTools::Debug("I: DEBUG: GameSession::gameMainloop: FINISHED");
}

void GameSession::timestepInput() {
    while (SDL_PollEvent(&e)){
        if (e.type == SDL_QUIT){
            quit = true;
        }
        if (e.type == SDL_KEYDOWN){
            switch (e.key.keysym.sym) {
            case SDLK_UP:
                board->getMothership()->moveForward();
                break;
            case SDLK_DOWN:
                break;
            case SDLK_LEFT:
                board->getMothership()->turnLeft();
                break;
            case SDLK_RIGHT:
                board->getMothership()->turnRight();
                break;
            default:
                break;
            }
        }
        if (e.type == SDL_KEYUP){
            switch (e.key.keysym.sym) {
            case SDLK_UP:
                board->getMothership()->stopForwardMovement();
                break;
            case SDLK_DOWN:
                break;
            case SDLK_LEFT:
                board->getMothership()->stopTurn();
                break;
            case SDLK_RIGHT:
                board->getMothership()->stopTurn();
                break;
            default:
                break;
            }
        }
        if (e.type == SDL_MOUSEBUTTONDOWN){
            quit = true;
        }
    }
}

void GameSession::placeActiveScreen() {
    double x = board->getMothership()->getX();
    double y = board->getMothership()->getY();
    as->setCenter(x, y);
}

void GameSession::timestepTimers() {
    // for every object
    // Tell to count inner clock forward

}

void GameSession::timestepEntities() {
    // for every object in active screen
    // update speed according to acceleration
    // collision?
    // update position according to speed (collision?)
    // collision?
    board->updateEntities();
}

void GameSession::timestepTriggers() {

}

void GameSession::timestepAI() {

}

void GameSession::timestepDraw() {

    std::vector<Section*> visibleSections = board->getVisibleSections();
    as->setTextureRects(visibleSections);

    // THIS NEEDS TO BE CHANGED
    SDL_RenderCopy(ren, controller->getTextures()->background, NULL, NULL);

    for (unsigned int i = 0; i < visibleSections.size(); ++i) {
        visibleSections[i]->draw(ren);
        if (showSectionLines) {
            visibleSections[i]->drawSectionLines(ren, as->getXOffset(), as->getYOffset());
        }
    }
    // For every unit in active section
    // for every unit in field of view
    // draw unit
}


} // namespace esControl