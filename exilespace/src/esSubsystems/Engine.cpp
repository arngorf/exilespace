
#include "Engine.hpp"

namespace esSubsystems {

Engine::Engine() {
    active = false;
}

void Engine::setNewSpeed(double* newDx, double* newDy, double angle, double throttle, double mass) {

    double x = *newDx;
    double y = *newDy;

    double maxVelocity = throttle*getMaxVelocity();

    double nangle = (-angle/ 180 + 1) * M_PI;

    double acceleration = getMaxAcceleration() / mass;

    if (active) {
        x += sin(nangle)*acceleration;
        y += cos(nangle)*acceleration;
    } else {
        x -= sin(nangle)*acceleration;
        y -= cos(nangle)*acceleration;
    }
    double newSpeed = sqrt(x*x + y*y);
    if (active) {
        if (newSpeed > maxVelocity) {
            (*newDx) = x * maxVelocity / newSpeed;
            (*newDy) = y * maxVelocity / newSpeed;
        } else {
            (*newDx) = x;
            (*newDy) = y;
        }
    }

    /*

    double dangle;

    if (active) {
        if (x == 0 && y == 0) {
            dangle = angle;
        } else {

            double curAngle = getAngle(x, y);

            if ( abs(curAngle - angle ) < abs( modAngle(curAngle + 180) - modAngle(angle + 180) ) ) {
                dangle = curAngle * (1-dTurn) + angle * dTurn;
            } else {
                dangle = modAngle(modAngle(curAngle + 180)*(1-dTurn) + modAngle(angle + 180)*dTurn - 180);
            }
        }
    } else {
        dangle = getAngle(x, y);
    }
    double nangle = (-dangle/ 180 + 1) * M_PI;
    double speed = sqrt(x*x + y*y);
    esTools::Debug("I: DEBUG: Test: angleRads = " + std::to_string(nangle) + " speed = " + std::to_string(speed));


    if (active) {
        speed += 0.01 * maxVelocity;
        if (speed > maxVelocity) {
            speed = maxVelocity;
        }
    }

    if (!active || speed > maxVelocity*1.01) {
        speed -= 0.01 * maxVelocity;
        if (speed < 0) {
            speed = 0;
        }
    }

    (*newDx) = sin(nangle)*speed;
    (*newDy) = cos(nangle)*speed;*/


}

} // namespace esSubsystems