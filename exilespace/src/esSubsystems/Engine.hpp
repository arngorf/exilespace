/*
 * Engine class
 *
 * Base Engine class.
 *
 */

#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <math.h>

#include "../esEntity/Entity.hpp"

namespace esSubsystems {

class Engine : public esEntity::Entity {
public:
    Engine();
    virtual ~Engine() {};

    virtual void activate() {active = true;};
    virtual void deactivate() {active = false;};
    virtual void setNewSpeed(double* newDx, double* newDy, double angle, double throttle, double dTurn);



    virtual void draw(SDL_Renderer *ren) = 0;

protected:
    virtual double getMaxAcceleration() = 0;
    virtual double getMaxVelocity() = 0;

private:

    bool active;

};

} // namespace esSubsystems

#endif // ENGINE_HPP