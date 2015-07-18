/*
 * Game Session class
 * Singleton class
 *
 * This class controls a game session.
 * Every game has a session.
 * Holds every information that needs to be static and needs saving and
 * loading. These include:
 * - Game settings
 * - Game progress
 *   + Units
 *   + Bases
 *   + Relations
 *   + Research
 *   + Mothership status
 *
 * This class is in charge of controlling the overall game flow
 * This includes making sure objects units effects and moved and
 * changed in accordance with the flow of the game.
 *
 * The primary splitting of this work is done through a game board consisting of sections.
 *
 */

#ifndef GAMESESSION_HPP
#define GAMESESSION_HPP

#include <vector>
#include "../esTools/Debug.hpp"
#include "Gameboard.hpp"
#include "ProgramController.hpp"
#include "../esView/ActiveScreen.hpp"

namespace esControl {

class ProgramController;

enum gameState
{
   INTRO,
   PLAYING,
};

class GameSession {
public:
    GameSession(ProgramController* controllerIn);
    ~GameSession();

    // Order Game Session to start the game.
    void start();

    // Mainloop for the intro screen
    void introScreenMainloop();

    // Mainloop for an active game
    void gameMainloop();

    // Timestep function for input event handling.
    void timestepInput();

    void placeActiveScreen();

    // Timestep function for timer updates like cooldowns etc.
    void timestepTimers();

    // Timestep function for physics event like movement, collisions etc.
    void timestepEntities();

    // Timestep function for game triggers, entering regions AI proximity alerts etc.
    void timestepTriggers();

    // Timestep function for making the AI have thoughts, strategies decisions etc.
    void timestepAI();

    // Timestep function for drawing all graphics
    void timestepDraw();

private:
    gameState state;

    ProgramController *controller;
    SDL_Renderer *ren;
    esView::ActiveScreen *as;

    SDL_Event e;

    Gameboard *board;

    bool showSectionLines;
    bool quit = false;
};

} // namespace esControl

#endif // GAMESESSION_HPP