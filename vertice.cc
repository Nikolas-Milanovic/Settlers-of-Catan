#include <string>
#include <iostream>

#include "vertice.h"
#include "Info.h"

// constructor
Vertice::Vertice(int pos) : name{std::to_string(pos)}, location{pos}, owner{-1}, vertexType{Residence::NoRes} {}

// gets Vertex type
Residence Vertice::getType()
{
    return vertexType;
}
// sets Vertex Name
void Vertice::setName(std::string s)
{
    name = s;
}
// sets Vertex Type
void Vertice::setType(Residence r)
{
    vertexType = r;
}
// gets Owner
int Vertice::getOwner()
{
    return owner;
}
// sets Owner
void Vertice::setOwner(int r)
{
    owner = r;
}
// gets Location
int Vertice::getLocation()
{
    return location;
}

// prints vertice
std::string Vertice::print()
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

// attaches Builder to Vertex
bool Vertice::attachBuilder(Observer *b)
{
    this->attach(b);
}

// Overrides from Subject
void Vertice::notify(Subject &whoNotified)
{
    // Checks if tile was set to goose
    if (whoNotified.getInfo().numResources == 0)
    {
        numResources = 0;
    }
    else if (vertexType == Residence::NoRes)
    {
        numResources = 0;
    }
    else if (vertexType == Residence::Basement)
    {
        numResources = 1;
    }
    else if (vertexType == Residence::House)
    {
        numResources = 2;
    }
    else if (vertexType == Residence::Tower)
    {
        numResources = 3;
    }
    else if (whoNotified.getInfo().resource == Resource::Park)
    {
        numResources = 0;
    }
    typeResources = whoNotified.getInfo().resource;
    this->notifyObservers();
}

// Overrides from Subject
std::string Vertice::getName()
{
    return name;
}

// Overrides from Subject
Info Vertice::getInfo() const
{
    return {
        typeResources,
        numResources};
}
