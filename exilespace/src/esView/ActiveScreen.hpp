/*
 * Active screen class
 *
 * This class controls what is actively shown on the screen.
 *
 */

#ifndef ACTIVESCREEN_HPP
#define ACTIVESCREEN_HPP

#include <vector>
#include "../esControl/Section.hpp"

namespace esView {

class ActiveScreen {

public:
    ActiveScreen(double xIn, double yIn, int wIn, int hIn);
    ~ActiveScreen();

    double getXOffset();
    double getYOffset();

    void setCenter(double xIn, double yIn);

    void setTextureRects(std::vector<esControl::Section*> visibleSections);

private:
    double x;
    double y;
    double w;
    double h;
    double cx;
    double cy;
    double xOffset;
    double yOffset;

};

} // namespace esView

#endif // ACTIVESCREEN_HPP