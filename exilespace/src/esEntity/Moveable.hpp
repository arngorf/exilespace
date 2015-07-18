/*
 * Moveable class
 *
 * Base Moveable class.
 *
 */

#ifndef MOVEABLE_HPP
#define MOVEABLE_HPP

#include <math.h>
#include <string>

#include "Entity.hpp"
#include "../esGraphics/Helpers.hpp"
#include "../esGraphics/Textures.hpp"

namespace esEntity {

class Moveable : public Entity {
public:
    Moveable(double xIn, double yIn);
    ~Moveable();

    // coords movement etc.
    double getX() {return center.x;};
    double getY() {return center.y;};
    virtual std::string getName() = 0;

    double getAngle(double dx, double dy);

    virtual void update();

    virtual void turnLeft();
    virtual void turnRight();
    virtual void stopTurn();
    virtual void moveForward();
    virtual void stopForwardMovement();

    // Graphics
    void setTextureRectangle(double xIn, double yIn);
    virtual void draw(SDL_Renderer *ren) = 0;

protected:
    double x;
    double y;
    double dx;
    double dy;

    double da;
    double angle;
    esGraphics::Point center;
    SDL_Point textureCenter;
    SDL_Rect* textureRect;
    SDL_Texture* texture;

};

} // namespace esEntity

#endif // MOVEABLE_HPP