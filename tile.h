#ifndef TILE_H
#define TILE_H

#include <string>
#include <set>

#include "subject.h"
#include "Info.h"

class Tile : public Subject
{
    int location;
    bool geese;
    Resource resource;
    int value;

public:
    Tile(int location, Resource resource, int value);

    // Gets/Sets
    int getLocation();
    int getValue();
    void setGeese(bool val);
    bool getGeese();

    // Printing Functions
    std::string printGeese();
    std::string printLocation();
    std::string printValue();
    std::string printResource();

    // Overide from Subject Class
    virtual Info getInfo() const override;
};

#endif
