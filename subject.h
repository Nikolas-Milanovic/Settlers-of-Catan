#ifndef SUBJECT_H
#define SUBJECT_H
#include <vector>
#include <string>

#include "Info.h"

class Observer;

class Subject
{
protected:
    std::vector<Observer *> observers;

public:
    void notifyObservers();
    void attach(Observer *o);
    virtual Info getInfo() const = 0;

    // Returns a string consisting of the name of each observer, followed by a single space.
    // If there are no observers, an empty string is returned.
    std::string getObserverNames();
};
#endif
