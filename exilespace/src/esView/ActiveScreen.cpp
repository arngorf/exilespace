
#include "ActiveScreen.hpp"

namespace esView {

ActiveScreen::ActiveScreen(double xIn, double yIn, int wIn, int hIn) {
    x = xIn;
    y = yIn;
    w = wIn;
    h = hIn;
    cx = xIn+wIn/2;
    cy = yIn+wIn/2;
}

ActiveScreen::~ActiveScreen() {

}

double ActiveScreen::getXOffset() {
    return x;
}

double ActiveScreen::getYOffset() {
    return y;
}

void ActiveScreen::setCenter(double xIn, double yIn) {
    x = xIn-w/2;
    y = yIn-h/2;
}

void ActiveScreen::setTextureRects(std::vector<esControl::Section*> visibleSections) {
    for (unsigned int i = 0; i < visibleSections.size(); ++i) {
        double xOffset = getXOffset();
        double yOffset = getYOffset();
        visibleSections[i]->setRectangles(xOffset, yOffset);
    }
}

} // namespace esView