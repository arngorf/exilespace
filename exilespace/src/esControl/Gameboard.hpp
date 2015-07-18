/*
 * Gameboard class
 *
 * This class controls and manages map sections.
 */

#ifndef GAMEBOARD_HPP
#define GAMEBOARD_HPP

#include <vector>
#include <string>
#include <unordered_map>

#include "../esTools/Debug.hpp"
#include "../esUnits/Mothership.hpp"
#include "../esUnits/TestShip.hpp"

#include "Section.hpp"

namespace esControl {

class Section;

typedef std::unordered_map<unsigned int, Section*> SectionMap;

class Gameboard {
public:
    Gameboard(esGraphics::Textures* texts);
    ~Gameboard();
    std::vector<Section*> getSections();
    void transferEntities();
    std::vector<Section*> getVisibleSections();

    /**
     * Compute unique identifier based on index coordinate.
     */
    unsigned int xyMap(int i, int j);

    /**
     * Returns true if a section is already existent at the given coordinates.
     */
    bool sectionExists(int i, int j);

    /**
     * Return section at the given coordinates if one exists, nullptr otherwise.
     */
    Section* getSection(int i, int j);

    /**
     * Connect pointers of two adjacent sections.
     */
    void connectSections(Section* sectionA, Section* sectionB);

    /**
     * Disconnect pointers of two adjacent connected sections.
     */
    void disconnectSections(Section* sectionA, Section* sectionB);

    Section* addSection(int i, int j);
    void placeSectionSquare(int i, int j, int width, int height);
    void placeStartLocation(int i, int j);

    void addTestShip(int x, int y);
    void addMothership(int x, int y);
    esUnits::Mothership* getMothership() {return mothership;};

    /**
     * Update every internal variable, position, timers etc. of every entity in every Section.
     */
    void updateEntities();



private:

    void placeEntityInSection(esEntity::Moveable* item);

    esUnits::Mothership* mothership;

    static const unsigned int sectionWidth = 500; //200;
    static const unsigned int sectionHeight = 500; //150;

    unsigned int nextSectionID;
    std::vector<Section*> sectionsVector;
    SectionMap sMap;

    esGraphics::Textures* textures;

};

} // namespace esControl

#endif // GAMEBOARD_HPP