/*
 * Ship class
 *
 * Base ship class.
 *
 */

#ifndef SHIP_HPP
#define SHIP_HPP

#include "../esEntity/Moveable.hpp"
#include "../esSubsystems/StandardEngine.hpp"

namespace esUnits {

class Ship : public esEntity::Moveable {
public:
    Ship(double xIn, double yIn);
    ~Ship(){};

    virtual std::string getName() = 0;

    virtual void UpdatePosition();
    virtual void draw(SDL_Renderer *ren) = 0;

protected:
    esSubsystems::Engine* mainEngine;

};

} // namespace esUnits

#endif // SHIP_HPP