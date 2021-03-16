#ifndef BUILDER_H
#define BUILDER_H

#include <iostream>
#include <ostream>
#include <vector>
#include <memory>
#include <string>

#include "dice.h"
#include "subject.h"
#include "observer.h"
#include "vertice.h"
#include "edge.h"

class Builder : public Observer
{
    // 0 blue, 1 red, 2 orange, 3 yellow
    //basement: 1 point, house: 2 points, tower: 3 points
    int numPoint;
    std::vector<int> gained;
    std::vector<int> resources;
    // [0]: brick, [1]: energy, [2]: glass, [3]: heat, [4]: WiFi
    Dice dice;
    int colour;
    void addPoint();             //done
    void resetResourcesGained(); //done

public:
    Builder(int col);
    void goose();                                 //take away half
    void gooseSteal(std::shared_ptr<Builder> &b); //what resource did you steal
    void setDie(Die d);                           //done
    Die getDieType();
    int getColour();
    bool buildBasement(std::unique_ptr<Vertice> &vertice); //done
    bool startGameBasement(std::unique_ptr<Vertice> &vertice);
    bool upgrade(std::unique_ptr<Vertice> &vertice);
    bool buildRoad(std::unique_ptr<Edge> &edge);                                 //done
    bool resourcesGained();                                                      //done
    bool trade(std::shared_ptr<Builder> &b, std::string give, std::string take); //done
    bool canSteal();                                                             //done
    std::string status();                                                        //done
    std::string printResourcesGained();
    int roll(int roll);
    int getPoints();
    std::string getResources();

    // overrides from Observer
    virtual void notify(Subject &whoNotified) override;
    virtual std::string getName() override;

    // METHODS ONLY USED FOR LOADING GAME FROM SAVE FILE
    void setRoad(std::unique_ptr<Edge> &edge);
    void setResidence(std::unique_ptr<Vertice> &vertice, std::string s);
    void setResources(int pos, int amt);
};

#endif
