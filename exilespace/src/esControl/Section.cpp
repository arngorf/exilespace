
#include "Section.hpp"

namespace esControl {

TTF_Font* Section::cleanText = nullptr;

Section::Section(unsigned int ID, int i, int j, int width, int height, Gameboard* gbIn) : SectionID_(ID), i_(i), j_(j), width_(width), height_(height), topLeftX_(i*width), topLeftY_(j*height) {

    esTools::Debug("I: DEBUG: Section::Section: CALLED");

    gb = gbIn;
    verbose = false;

    currentEntityHead = nullptr;
    transferEntitiesHead = nullptr;
    transferEntitiesTail = nullptr;

    // Connect to existing sections

    for (int dj = -1; dj < 2; ++dj) {
        for (int di = -1; di < 2; ++di) {
            if (dj == 0 && di == 0) {
                continue;
            }
            Section* neighbourSection = gb->getSection(i+di, j+dj);
            if (neighbourSection != nullptr) {
                gb->connectSections(this, neighbourSection);
            }
        }
    }

    // INIT of section visible boundary lines and index text

    SDL_Color green = {0, 255, 0, 100};

    if (cleanText == nullptr) {
        cleanText = TTF_OpenFont("../resources/fonts/helvetica-normal.ttf", 10);
    }
    std::string sectionIndexString = std::to_string(i) + "," + std::to_string(j);

    Message_rect.w = 8*sectionIndexString.length();
    Message_rect.h = 10;

    surfaceMessage = TTF_RenderText_Solid(cleanText, sectionIndexString.c_str(), green);

    esTools::Debug("I: DEBUG: Section::Section: FINISHED");
}

Section::~Section() {

    // Connect to existing sections

    for (int dj = -1; dj < 2; ++dj) {
        for (int di = -1; di < 2; ++di) {
            if (dj == 0 && di == 0) {
                continue;
            }
            Section* neighbourSection = gb->getSection(i_+di, j_+dj);
            if (neighbourSection != nullptr) {
                gb->disconnectSections(this, neighbourSection);
            }
        }
    }

}

void Section::addEntity(esEntity::Moveable* item) {

    esTools::Debug("I: DEBUG: Section::addEntity: CALLED");

    SectionEntity* newEntityPtr = new SectionEntity;
    newEntityPtr->item = item;
    transferEntity(newEntityPtr);

    esTools::Debug("I: DEBUG: Section::addEntity: FINISHED");
}

bool Section::isInside(int x, int y) {
    int nx = x - topLeftX_;
    int ny = y - topLeftY_;

    return (nx >= 0 && nx < width_ && ny >= 0 && ny < height_);

}

direction Section::placement(int x, int y) {
    int nx = x - topLeftX_;
    int ny = y - topLeftY_;

    if (nx < 0) {
        if (ny < 0) {
            return NORTHWEST;
        } else if (ny >= height_) {
            return SOUTHWEST;
        } else {
            return WEST;
        }
    } else if (nx >= width_) {
        if (ny < 0) {
            return NORTHEAST;
        } else if (ny >= height_) {
            return SOUTHEAST;
        } else {
            return EAST;
        }
    } else {
        if (ny < 0) {
            return NORTH;
        } else if (ny >= height_) {
            return SOUTH;
        } else {
            return CENTER;
        }
    }
}

void Section::transferEntity(SectionEntity* entityPtr) {

    esTools::Debug("I: DEBUG: Section::transferEntity: CALLED");
    esTools::Debug("I: DEBUG: Section::transferEntity: Starting transfer of entity at section i: " + std::to_string(i_) + " j: " + std::to_string(j_));

    entityPtr->link = transferEntitiesHead;
    transferEntitiesHead = entityPtr;
    if (transferEntitiesTail == nullptr) {
        transferEntitiesTail = entityPtr;
    }

    esTools::Debug("I: DEBUG: Section::transferEntity: FINISHED");
}

void Section::completeTransfer() {

    if (transferEntitiesHead != nullptr) {
        esTools::Debug("I: DEBUG: Transferring entities at i: " + std::to_string(i_) + " j: " + std::to_string(j_));

        transferEntitiesTail->link = currentEntityHead;
        currentEntityHead = transferEntitiesHead;

        transferEntitiesHead = nullptr;
        transferEntitiesTail = nullptr;
    }
}

bool Section::isUnused() {
    return (transferEntitiesHead == nullptr && currentEntityHead == nullptr);
}

void Section::updateEntities() {

    SectionEntity* previousEntity = nullptr;
    SectionEntity* currentEntity = currentEntityHead;

    while (currentEntity != nullptr) {

        currentEntity->item->update();

        int x = currentEntity->item->getX();
        int y = currentEntity->item->getY();
        direction place = placement(x, y);

        bool moving = (place != CENTER);
        bool firstNode = (previousEntity == nullptr);

        if (firstNode) {
            if (moving) {
                currentEntity = currentEntity->link;
                Section* dstSection = getSection(place);
                if (dstSection != nullptr) {
                    dstSection->transferEntity(currentEntityHead);
                } else {
                    int ni = (int) floor(x / double(width_));
                    int nj = (int) floor(y / double(height_));

                    Section* newSection = nullptr;
                    newSection = gb->getSection(ni, nj);
                    if (newSection == nullptr) {
                        esTools::Debug("I: DEBUG: Sending entity to non-existent map section, Adding new section!");
                        newSection = gb->addSection(ni, nj);
                    } else {
                        esTools::Debug("I: DEBUG: Sending entity to non-connected map section, Adding link!");
                    }

                    gb->connectSections(this, newSection);
                    newSection->transferEntity(currentEntityHead);

                }
                currentEntityHead = currentEntity;

            } else {
                currentEntity = currentEntity->link;
                previousEntity = currentEntityHead;
            }
        } else {
            if (moving) {
                currentEntity = currentEntity->link;
                Section* dstSection = getSection(place);
                if (dstSection != nullptr) {
                    getSection(place)->transferEntity(previousEntity->link);
                } else {
                    int ni = int(x / width_);
                    int nj = int(y / height_);

                    Section* newSection = nullptr;
                    newSection = gb->getSection(ni, nj);
                    if (newSection == nullptr) {
                        esTools::Debug("I: DEBUG: Sending entity to non-existent map section, Adding new section!");
                        newSection = gb->addSection(ni, nj);
                    } else {
                        esTools::Debug("I: DEBUG: Sending entity to non-connected map section, Adding link!");
                    }

                    gb->connectSections(this, newSection);
                    newSection->transferEntity(previousEntity->link);


                }
                previousEntity->link = currentEntity;
            } else {
                currentEntity = currentEntity->link;
                previousEntity = previousEntity->link;
            }
        }
    }
}

void Section::setRectangles(double xOffset, double yOffset) {

    SectionEntity* currentEntity = currentEntityHead;

    while (currentEntity != nullptr) {

        double x = currentEntity->item->getX();
        double y = currentEntity->item->getY();
        currentEntity->item->setTextureRectangle(x - xOffset, y - yOffset);

        currentEntity = currentEntity->link;
    }
}

void Section::draw(SDL_Renderer *ren) {

    SectionEntity* currentEntity = currentEntityHead;

    while (currentEntity != nullptr) {

        currentEntity->item->draw(ren);

        currentEntity = currentEntity->link;
    }
}

void Section::drawSectionLines(SDL_Renderer *ren, int xOffset, int yOffset) {

    if (currentEntityHead != nullptr) {
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 100);
    } else {
        SDL_SetRenderDrawColor(ren, 0, 255, 0, 100);
    }

    // Compute upper left corner of section in active screen
    int nx = topLeftX_ - xOffset;
    int ny = topLeftY_ - yOffset;

    // Draw section boundary lines
    SDL_RenderDrawLine(ren, nx, ny, nx + width_ - 1, ny);
    SDL_RenderDrawLine(ren, nx, ny, nx, ny + height_ - 1);
    SDL_RenderDrawLine(ren, nx + width_ - 1, ny,nx + width_ - 1, ny + height_ - 1);
    SDL_RenderDrawLine(ren, nx, ny + height_-1,nx + width_ - 1, ny + height_ - 1);

    if (Message == nullptr) {
        Message = SDL_CreateTextureFromSurface(ren, surfaceMessage);
    }
    if (cleanText == nullptr) {
        esTools::Debug("E: DEBUG: TTF TextType Missing!");
    } else {
        // Draw index values

        Message_rect.x = nx+2;
        Message_rect.y = ny+2;

        SDL_RenderCopy(ren, Message, NULL, &Message_rect);
    }
}

Section* Section::getSection(direction dir) {
    switch (dir) {
    case CENTER:
        std::cout << "W: DEBUG: Getting center pointer which is always null" << std::endl;
        return nullptr;
    case NORTH:
        return N;
    case SOUTH:
        return S;
    case EAST:
        return E;
    case WEST:
        return W;
    case NORTHEAST:
        return NE;
    case NORTHWEST:
        return NW;
    case SOUTHEAST:
        return SE;
    case SOUTHWEST:
        return SW;
    default:
        std::cout << "W: DEBUG: Invalid invalid direction" << std::endl;
        return nullptr;
    }

}


} // namespace esControl