#ifndef DICE_H
#define DICE_H

enum class Die
{
    Loaded,
    Fair
};

class Dice
{
    Die diceType;

public:
    void setType(Die type);
    Die getType();
    int roll(int n);
};

#endif
