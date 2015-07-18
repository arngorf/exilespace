/*
 * Mother ship class
 *
 */

#ifndef MOTHERSHIP_HPP
#define MOTHERSHIP_HPP

#include "Ship.hpp"

namespace esUnits {

class Mothership : public Ship {
public:
    Mothership(double xIn, double yIn, esGraphics::Textures* texts);
    ~Mothership();

    std::string getName();

    // coords movement etc.
    void moveForward();
    void stopForwardMovement();
    void update();

    // Graphics
    void draw(SDL_Renderer *ren);
private:
    std::string name;

};

} // namespace esUnits

#endif // MOTHERSHIP_HPP