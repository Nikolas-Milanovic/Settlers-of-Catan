#ifndef VERTICE_H
#define VERTICE_H

#include <string>
#include <vector>

#include "Info.h"
#include "subject.h"
#include "observer.h"

class Vertice : public Subject, public Observer
{
    int location;
    int numResources;
    int owner;
    std::string name;
    Resource typeResources;
    Residence vertexType;

public:
    Vertice(int pos);
    //get functions
    int getOwner();
    void setOwner(int r);
    int getLocation();
    Residence getType();
    void setType(Residence r);
    bool attachBuilder(Observer *b);
    void setName(std::string s);
    // print function
    std::string print();

    // Overrides from Observer/Subject
    virtual void notify(Subject &whoNotified) override;
    virtual std::string getName() override;
    virtual Info getInfo() const override;
};

#endif
