

#include "StandardEngine.hpp"

namespace esSubsystems {

StandardEngine::StandardEngine() {
    maxA = 0.5;
    maxV = 2.0;
}

StandardEngine::~StandardEngine() {

}

void StandardEngine::draw(SDL_Renderer *ren) {

}

double StandardEngine::getMaxAcceleration() {
    return maxA;
}

double StandardEngine::getMaxVelocity() {
    return maxV;
}

} // namespace esSubsystems