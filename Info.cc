#include <string>

#include "Info.h"

Resource intToResource(int resource)
{
    if (resource == 0)
    {
        return Resource::Brick;
    }
    else if (resource == 1)
    {
        return Resource::Energy;
    }
    else if (resource == 2)
    {
        return Resource::Glass;
    }
    else if (resource == 3)
    {
        return Resource::Heat;
    }
    else if (resource == 5)
    {
        return Resource::Park;
    }
    else
    {
        return Resource::Wifi;
    }
}

int stringToInt(std::string s)
{
    if (s == "Brick")
    {
        return 0;
    }
    else if (s == "Energy")
    {
        return 1;
    }
    else if (s == "Glass")
    {
        return 2;
    }
    else if (s == "Heat")
    {
        return 3;
    }
    else if (s == "Wifi")
    {
        return 4;
    }
    return -1;
}

int resourceToInteger(Resource r)
{
    if (Resource::Brick == r)
    {
        return 0;
    }
    else if (Resource::Energy == r)
    {
        return 1;
    }
    else if (Resource::Glass == r)
    {
        return 2;
    }
    else if (Resource::Heat == r)
    {
        return 3;
    }
    else if (Resource::Wifi == r)
    {
        return 4;
    }
    else
    {
        return 5;
    }
}
