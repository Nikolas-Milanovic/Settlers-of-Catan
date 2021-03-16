#ifndef _OBSERVER_H_
#define _OBSERVER_H_
#include <string>

// DO NOT MODIFY THIS FILE!

class Subject;

class Observer
{
public:
    // Pass in the Subject that called the notify method.
    virtual void notify(Subject &whoNotified) = 0;
    virtual std::string getName() = 0;
    virtual ~Observer() = default;
};

#endif
