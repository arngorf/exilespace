/*
 * Entity class
 *
 * Base Entity class.
 *
 */

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SDL.h>

#include "../esTools/Debug.hpp"

namespace esEntity {

class Entity {
public:
    Entity(){};
    ~Entity(){};

    double modAngle(double angle);
    double getAngle(double dx, double dy);

    virtual void draw(SDL_Renderer *ren) = 0;
private:
};

} // namespace esEntity

#endif // ENTITY_HPP