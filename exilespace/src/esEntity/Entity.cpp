
#include "Entity.hpp"

namespace esEntity {

double Entity::modAngle(double angle) {
    while (angle >= 360) {
        angle -= 360;
    } while (angle < 0) {
        angle += 360;
    }
    return angle;
}

double Entity::getAngle(double dx, double dy) {
    double angle;
    if (dx != 0 || dy != 0) {

        double theta = atan2(-dx, dy);

        angle = theta * 180 / M_PI + 180;

        angle = modAngle(angle);

    } else {
        esTools::Debug("I: DEBUG: Test: x = " + std::to_string(dx) + " y = " + std::to_string(dy));
        esTools::Debug("W: DEBUG: Moveable::getAngle: Getting angle with a zero vector!");
        angle = 0;
    }
    return angle;
}

} // namespace esEntity