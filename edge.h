#ifndef EDGE_H
#define EDGE_H
#include <string>
#include <vector>

#include "Info.h"

class Board;

class Edge
{
    std::string name;
    int location;
    int owner;

public:
    Edge(int location);
    // Getters
    int getLocation();
    int getOwner();
    std::string getName();
    void buildRoad(int colour);

    // Print function
    std::string print();
};

#endif
