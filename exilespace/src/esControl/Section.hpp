/*
 * Section class
 *
 * This class represents a map region and methods for moving units
 * and other imaginable "things" between map sections.
 */

#ifndef SECTION_HPP
#define SECTION_HPP

#include <string>

#include "Gameboard.hpp"
#include "../esTools/Debug.hpp"
#include "../esEntity/Moveable.hpp"

namespace esControl {

class Gameboard; // Forward declaration.

enum direction
{
   NORTH,
   SOUTH,
   EAST,
   WEST,
   NORTHEAST,
   NORTHWEST,
   SOUTHEAST,
   SOUTHWEST,
   CENTER,
};

/*
 * Linked list struct containing a moveable map object such as a unit or object.
 *
 */
struct SectionEntity {
    esEntity::Moveable* item;
    SectionEntity* link;
};

class Gameboard;

class Section {
public:
    Section(unsigned int ID, int iIn, int jIn, int widthIn, int heightIn, Gameboard* gbIn);
    ~Section();

    int getI() {return i_;};
    int getJ() {return j_;};

    bool isInside(int x, int y);
    direction placement(int x, int y);
    void addEntity(esEntity::Moveable* item);

    /**
     * Queue entity for transfer to another Section;
     */
    void transferEntity(SectionEntity* entityPtr);

    /**
     * Complete transfer of all entities queued for transfer to this section.
     */
    void completeTransfer();

    /**
     * Function to check if section is unused and thus can be safely deleted.
     */
    bool isUnused();

    /**
     * Update every internal variable, position, timers etc. of every entity in this Section.
     */
    void updateEntities();

    /**
     * Modify all texture rectangles.
     * OBS: This may need to be rewritten.
     */
    void setRectangles(double xOffset, double yOffset);

    /**
     * Draw all "thing" in the current section
     * OBS: This will need to be rewritten to account for drawing objects in correct order.
     */
    void draw(SDL_Renderer *ren);

    /**
     * Little function to draw section lines and section indices if enabled.
     */
    void drawSectionLines(SDL_Renderer *ren, int xOffset, int yOffset);


    Section *N = nullptr, *S = nullptr, *E = nullptr, *W = nullptr, *NE = nullptr, *NW = nullptr, *SE = nullptr, *SW = nullptr;

private:
    const unsigned int SectionID_;
    const int i_;
    const int j_;
    const int width_;
    const int height_;
    const int topLeftX_;
    const int topLeftY_;

    Gameboard* gb;

    Section* getSection(direction dir);

    SectionEntity* currentEntityHead; // This is the head of a linked list containing Units, Objects etc.
    SectionEntity* transferEntitiesHead; // This is the head of a linked list containing Units to be transfered to the section.
    SectionEntity* transferEntitiesTail;



    bool verbose;

    static TTF_Font* cleanText;
    SDL_Surface* surfaceMessage = nullptr;
    SDL_Texture* Message = nullptr;
    SDL_Rect Message_rect;

};

} // namespace esControl

#endif // SECTION_HPP