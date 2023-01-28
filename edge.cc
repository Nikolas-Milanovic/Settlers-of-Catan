#include <string>

#include "edge.h"

// constructor
Edge::Edge(int location) : name{std::to_string(location)}, location{location}, owner{-1} {}

// gets Owner
int Edge::getOwner()
{
    return owner;
}

// gets Name
std::string Edge::getName()
{
    return name;
}

// gets Location
int Edge::getLocation()
{
    return location;
}

// Builds road, sets owner to builder, updates name
void Edge::buildRoad(int builder)
{
    std::vector<std::string> builders = {{"B"}, {"R"}, {"O"}, {"Y"}};
    owner = builder;
    name = std::string(builders[builder]) + "R";
}

// prints Edge
std::string Edge::print()
{
    if (owner == -1)
    {
        if (location < 10)
        {
            return std::string(" ") + std::to_string(location);
        }
        else
        {
            return std::to_string(location);
        }
    }
    else
    {
        return name;
    }
}
