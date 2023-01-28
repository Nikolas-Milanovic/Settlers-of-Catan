#include "dice.h"
#include <iostream>
#include <stdlib.h>

void Dice::setType(Die type)
{
    diceType = type;
}

Die Dice::getType()
{
    return diceType;
}

int Dice::roll(int n)
{
    if (diceType == Die::Loaded)
    {
        if (2 <= n && n <= 12)
        {
            return n;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        srand((unsigned)time(0));
        int roll1 = std::rand() % 6 + 1;
        int roll2 = std::rand() % 6 + 1;
        return roll1 + roll2;
    }
}
