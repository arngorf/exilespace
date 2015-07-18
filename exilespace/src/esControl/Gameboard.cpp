

#include "Gameboard.hpp"

namespace esControl {

Gameboard::Gameboard(esGraphics::Textures* texts) {

    esTools::Debug("I: DEBUG: Gameboard::Gameboard: CALLED");

    if (texts != nullptr){
        textures = texts;
    } else {
        esTools::Debug("E: DEBUG: Gameboard::Gameboard: Textures not initialized!");
    }

    mothership = nullptr;

    esTools::Debug("I: DEBUG: Gameboard::Gameboard: FINISHED");
}

Gameboard::~Gameboard() {
    for (unsigned int i = 0; i < sectionsVector.size(); ++i) {
        delete sectionsVector[i];
    }
}

std::vector<Section*> Gameboard::getSections() {
    return sectionsVector;
}

void Gameboard::transferEntities() {

    for (unsigned int i = 0; i < sectionsVector.size(); ++i){
        sectionsVector[i]->completeTransfer();
    }
}

std::vector<Section*> Gameboard::getVisibleSections() {
    // This needs to be changed.
    return getSections();
}

unsigned int Gameboard::xyMap(int i, int j) {

    int n = std::abs(i) + std::abs(j);
    int base = 2*n*(n-1);

    if (i > 0 && j >= 0) {
        return base + i;
    } else if (i <= 0 && j > 0) {
        return base + n + j;
    } else if (i < 0 && j <= 0) {
        return base + 2*n - i;
    }
    return base + 3*n - j;
}

bool Gameboard::sectionExists(int i, int j) {
    if (sMap.count(xyMap(i,j)) == 1) {
        return true;
    } else {
        return false;
    }
}

Section* Gameboard::getSection(int i, int j) {
    SectionMap::const_iterator got = sMap.find(xyMap(i, j));
    if (got == sMap.end()) {
        return nullptr;
    } else {
        return got->second;
    }
}

void Gameboard::connectSections(Section* sectionA, Section* sectionB) {

    esTools::Debug("I: DEBUG: Gameboard::connectSections: CALLED");

    if (sectionA == nullptr || sectionB == nullptr) {
        esTools::Debug("E: DEBUG: Attempting to connect section to nullptr!");
    }
    int dx = sectionB->getI() - sectionA->getI();
    int dy = sectionB->getJ() - sectionA->getJ();

    if (std::abs(dx) > 1 || std::abs(dy) > 1) {
        esTools::Debug("E: DEBUG: Attempting to connect non-adjacent sections!");
        std::cout << dx << ", " << dy << std::endl;
    } else {
        std::cout << dx << ", " << dy << std::endl;

        if (dy < 0) {
            // SOUTH
            if (dx > 0) {
                // EAST
                sectionA->SE = sectionB;
                sectionB->NW = sectionA;
            } else if (dx < 0) {
                // WEST
                sectionA->SW = sectionB;
                sectionB->NE = sectionA;
            } else {
                // NO HORIZONTAL MOVEMENT
                sectionA->S = sectionB;
                sectionB->N = sectionA;
            }
        } else if (dy > 0) {
            // NORTH
            if (dx > 0) {
                // EAST
                sectionA->NE = sectionB;
                sectionB->SW = sectionA;
            } else if (dx < 0) {
                // WEST
                sectionA->NW = sectionB;
                sectionB->SE = sectionA;
            } else {
                // NO HORIZONTAL MOVEMENT
                sectionA->N = sectionB;
                sectionB->S = sectionA;
            }
        } else {
            // NO VERTICAL MOVEMENT
            if (dx > 0) {
                // EAST
                sectionA->E = sectionB;
                sectionB->W = sectionA;
            } else if (dx < 0) {
                // WEST
                sectionA->W = sectionB;
                sectionB->E = sectionA;
            } else {
                // NO HORIZONTAL MOVEMENT
                esTools::Debug("E: DEBUG: Attempting to connect section to itself!");
            }
        }
    }
    esTools::Debug("I: DEBUG: Gameboard::connectSections: FINISHED");
}

void Gameboard::disconnectSections(Section* sectionA, Section* sectionB) {

    esTools::Debug("I: DEBUG: Gameboard::disconnectSections: CALLED");

    if (sectionA == nullptr || sectionB == nullptr) {
        esTools::Debug("E: DEBUG: Attempting to disconnect section from nullptr!");
    }
    int dx = sectionB->getI() - sectionA->getI();
    int dy = sectionB->getJ() - sectionA->getJ();

    if (dy < 0) {
        // SOUTH
        if (dx > 0) {
            // EAST
            sectionA->SE = nullptr;
            sectionB->NW = nullptr;
        } else if (dx < 0) {
            // WEST
            sectionA->SW = nullptr;
            sectionB->NE = nullptr;
        } else {
            // NO HORIZONTAL MOVEMENT
            sectionA->S = nullptr;
            sectionB->N = nullptr;
        }
    } else if (dy > 0) {
        // NORTH
        if (dx > 0) {
            // EAST
            sectionA->NE = nullptr;
            sectionB->SW = nullptr;
        } else if (dx < 0) {
            // WEST
            sectionA->NW = nullptr;
            sectionB->SE = nullptr;
        } else {
            // NO HORIZONTAL MOVEMENT
            sectionA->N = nullptr;
            sectionB->S = nullptr;
        }
    } else {
        // NO VERTICAL MOVEMENT
        if (dx > 0) {
            // EAST
            sectionA->E = nullptr;
            sectionB->W = nullptr;
        } else if (dx < 0) {
            // WEST
            sectionA->W = nullptr;
            sectionB->E = nullptr;
        } else {
            // NO HORIZONTAL MOVEMENT
            esTools::Debug("E: DEBUG: Attempting to disconnect section from itself!");
        }
    }
    esTools::Debug("I: DEBUG: Gameboard::disconnectSections: FINISHED");
}

Section* Gameboard::addSection(int i, int j) {

    esTools::Debug("I: DEBUG: Gameboard::addSection: CALLED");

    unsigned int SID = xyMap(i,j);
    if (!sectionExists(i,j)) {

        Section* newSection = new Section(SID, i, j, sectionWidth, sectionHeight, this);
        sectionsVector.push_back(newSection);
        sMap[SID] = newSection;
        return newSection;
    } else {
        return getSection(i, j);
    }
    esTools::Debug("I: DEBUG: Gameboard::addSection: FINISHED");
}

void Gameboard::placeSectionSquare(int iIn, int jIn, int width, int height) {
    for (int j = iIn; j < (height + jIn); ++j) {
        for (int i = iIn; i < (width + iIn); ++i){
            addSection(i, j);
        }
    }
}

void Gameboard::placeStartLocation(int i, int j) {
    placeSectionSquare(i, j, 3, 3);
    addMothership(i*sectionWidth + sectionWidth/2, j*sectionHeight + sectionHeight/2);
}

void Gameboard::addTestShip(int x, int y) {
    esUnits::TestShip* newShip = nullptr;
    if (textures != nullptr) {
        newShip = new esUnits::TestShip(x, y, textures);
    } else {
        esTools::Debug("E: DEBUG: Gameboard::addTestShip: Textures not initialized!");
    }
    if (newShip != nullptr) {
        placeEntityInSection(newShip);
    } else {
        esTools::Debug("E: DEBUG: Failed to initialize TestShip!");
    }
}

void Gameboard::placeEntityInSection(esEntity::Moveable* item) {

    esTools::Debug("I: DEBUG: Gameboard::placeEntityInSection: CALLED placing: " + item->getName());

    if (item == nullptr) {
        esTools::Debug("E: DEBUG: Attempting to place nullptr to Section!");
    }

    bool unplaced = true;
    for (unsigned int i = 0; i < sectionsVector.size(); ++i) {
        if (sectionsVector[i]->isInside(item->getX(),item->getY())) {
            sectionsVector[i]->addEntity(item);
            unplaced = false;
            break;
        }
    }
    if (unplaced) {
        esTools::Debug("E: DEBUG: Could not find section for entity!");
    }
    esTools::Debug("I: DEBUG: Gameboard::placeEntityInSection: FINISHED");
}

void Gameboard::addMothership(int x, int y) {

    esTools::Debug("I: DEBUG: Gameboard::addMothership: CALLED");

    if (mothership != nullptr) {
        esTools::Debug("E: DEBUG: Creating Mothership again!");
    }
    if (textures != nullptr) {
        mothership = new esUnits::Mothership(x, y, textures);
    } else {
        esTools::Debug("E: DEBUG: Gameboard::addMothership: Textures not initialized!");
    }
    placeEntityInSection(mothership);

    esTools::Debug("I: DEBUG: Gameboard::addMothership: FINISHED");

}

void Gameboard::updateEntities() {
    for (unsigned int i = 0; i < sectionsVector.size(); ++i) {
        sectionsVector[i]->updateEntities();

    }
}





} // namespace esControl