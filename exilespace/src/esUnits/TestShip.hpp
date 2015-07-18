/*
 * Test ship class
 *
 */

#ifndef TESTSHIP_HPP
#define TESTSHIP_HPP

#include "Ship.hpp"

namespace esUnits {

class TestShip : public Ship {
public:
    TestShip(double xIn, double yIn, esGraphics::Textures* texts);
    ~TestShip();

    std::string getName();

    void draw(SDL_Renderer *ren);
private:
    std::string name;

};

} // namespace esUnits

#endif // TESTSHIP_HPP