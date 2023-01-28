#include "subject.h"
#include "observer.h"
#include <vector>
#include <algorithm>
#include <string>

void Subject::attach(Observer *o) { observers.emplace_back(o); }

void Subject::notifyObservers()
{
    for (auto &ob : observers)
        ob->notify(*this);
}

std::string Subject::getObserverNames()
{
    std::string nameList{};
    std::vector<std::string> names{};
    for (auto &ob : observers)
        names.push_back(ob->getName());
    std::sort(names.begin(), names.end());
    for (auto n : names)
        nameList += n + " ";
    return nameList;
}
