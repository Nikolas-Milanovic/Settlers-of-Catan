#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <ostream>
#include <vector>
#include <memory>
#include <string>
#include <set>

#include "vertice.h"
#include "edge.h"
#include "tile.h"
#include "builder.h"
#include "dice.h"

class Board
{
    std::vector<std::unique_ptr<Vertice>> vertices;
    std::vector<std::unique_ptr<Edge>> edges;
    std::vector<std::unique_ptr<Tile>> tiles;
    std::vector<std::shared_ptr<Builder>> builders;
    std::set<int> toStealFrom;
    std::shared_ptr<Builder> curBuilder;
	
    int rollValue(int rolled);	
    bool valid_road(int r);
    bool valid_Basement(int b, bool beginning_res);
    std::string integerToColour(int builder);
    int stringToInteger(std::string builder);
    int findCommonVertice(std::vector<int> v1, std::vector<int> v2);

public:
    
    Board();
    void gooseRolled();
    bool playGoose(int location);
    bool gooseSteal(std::string b);
    bool checkgoose(int n);
    void load();
    void fair();
    Die getDieType();
    int roll(int rolled = 1);
    void status();
    void printResidence();
    bool build_road(int road);
    bool improve(int res);
    bool build_res(int res, bool beginning_res = false);
    bool tradeLimitations(std::string tradeBuilder, std::string give, std::string take);
    bool trade(std::string trader, std::string give, std::string take);
    void next(bool print);
    void prev(bool print);
    bool win();
    std::string getCurColour();

    // Printing & Loading/Saving Functions
    void loadFromRandom(unsigned int seed);
    void print();
    void loadFromSave(std::string save);
    void loadFromFile(std::string file);
    void createVE();
    void attachVE();
    void save(std::string file);
    void setBuilder(int b, std::string line);
    void createBuilders();
    void reset();
};

#endif
