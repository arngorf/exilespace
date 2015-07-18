/*
 * Standard Engine class
 *
 */

#ifndef STANDARDENGINE_HPP
#define STANDARDENGINE_HPP

#include "Engine.hpp"

namespace esSubsystems {

class StandardEngine : public Engine {
public:
    StandardEngine();
    ~StandardEngine();

    void draw(SDL_Renderer *ren);

private:

    double getMaxAcceleration();
    double getMaxVelocity();

    double maxV;
    double maxA;
};

} // namespace esSubsystems

#endif // STANDARDENGINE_HPP