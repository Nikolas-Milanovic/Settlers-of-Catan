#include <string>
#include <cmath>

#include "builder.h"
#include "Info.h"

std::string addSpaces(int n)
{
    if (0 == n)
    {
        return "";
    }
    return " " + addSpaces(n - 1);
}

// Constructor
Builder::Builder(int col) : colour{col}, numPoint{0}
{
    for (int i = 0; i < 5; i++)
    {
        resources.emplace_back(0);
        gained.emplace_back(0);
    }
    setDie(Die::Loaded);
}

// sets die type
void Builder::setDie(Die d)
{
    dice.setType(d); //From Dice class
}

// gets Builder Colour
int Builder::getColour()
{
    return colour;
}
// gets Die type
Die Builder::getDieType()
{
    return dice.getType();
}
// adds 1 point
void Builder::addPoint()
{
    numPoint += 1;
}
// gets Points
int Builder::getPoints()
{
    return numPoint;
}

// resets resources gained
void Builder::resetResourcesGained()
{
    for (int i = 0; i < 5; i++)
    {
        gained[i] = 0;
    }
}

// builds basement DURING TURN FUNCTION
bool Builder::buildBasement(std::unique_ptr<Vertice> &vertice)
{
    //brick, energy, glass, wifi
    if (resources[0] < 1 || resources[1] < 1 || resources[2] < 1 || resources[4] < 1)
    {
        std::cout << "You do not have enough resources." << std::endl;
        return false;
    }

    resources[0] -= 1;
    resources[1] -= 1;
    resources[2] -= 1;
    resources[4] -= 1;

    vertice.get()->setOwner(colour);
    std::string name = this->getName();
    vertice.get()->setType(Residence::Basement);
    vertice.get()->setName(name.substr(0, 1) + "B");
    vertice.get()->attachBuilder(this);
    addPoint();
    return true;
}

bool Builder::upgrade(std::unique_ptr<Vertice> &vertice)
{
    std::string name = this->getName();
    if (vertice.get()->getType() == Residence::Basement)
    {
        if (resources[2] < 2 || resources[3] < 3)
        {
            std::cout << "You do not have enough resources." << std::endl;
            return false;
        }

        resources[2] -= 2;
        resources[3] -= 3;
        vertice.get()->setType(Residence::House);
        vertice.get()->setName(name.substr(0, 1) + "H");
        addPoint();
        std::cout << "Successfully upgraded to a House at " + std::to_string(vertice.get()->getLocation()) << std::endl;
        return true;
    }
    else
    {
        if (resources[0] < 3 || resources[1] < 2 || resources[2] < 2 || resources[4] < 1 || resources[3] < 2)
        {
            std::cout << "You do not have enough resources." << std::endl;
            return false;
        }
        resources[0] -= 3;
        resources[1] -= 2;
        resources[2] -= 2;
        resources[4] -= 1;
        resources[3] -= 2;
        vertice.get()->setType(Residence::Tower);
        vertice.get()->setName(name.substr(0, 1) + "T");
        addPoint();
        std::cout << "Successfully upgraded to a Tower at " + std::to_string(vertice.get()->getLocation()) << std::endl;
        return true;
    }
}

bool Builder::buildRoad(std::unique_ptr<Edge> &edge)
{
    //buildroad
    //heat and wifi
    if (resources[3] < 1 || resources[4] < 1)
    {
        std::cout << "You do not have enough resources." << std::endl;
        return false;
    }

    resources[3] -= 1;
    resources[4] -= 1;
    edge.get()->buildRoad(colour);
    return true;
}

bool Builder::resourcesGained()
{
    int sum = 0;
    for (int i = 0; i < 5; i++)
    {
        sum += gained[i];
    }
    if (sum <= 0)
    {
        return false;
    }
    return true;
}

bool Builder::trade(std::shared_ptr<Builder> &b, std::string give, std::string take)
{
    int give_resource = stringToInt(give);

    int take_resource = stringToInt(take);

    if (resources[give_resource] < 1 || b.get()->resources[take_resource] < 1)
    {
        std::cout << "You do not have enough resources." << std::endl;
        return false;
    }

    resources[give_resource] -= 1;
    b.get()->resources[give_resource] += 1;
    resources[take_resource] += 1;
    b.get()->resources[take_resource] -= 1;
    return true;
}

bool Builder::canSteal()
{
    for (int i = 0; i < 5; i++)
    {
        if (resources[i] != 0)
        {
            return true;
        }
    }
    return false;
}

std::string Builder::status()
{
    std::string stat = "";
    std::string col = this->getName();

    int diff = 6 - col.length(); //To create and even display
    stat += col + addSpaces(diff) + " has ";
    stat += std::to_string(numPoint) + " building points, ";
    stat += std::to_string(resources[0]) + " brick, ";
    stat += std::to_string(resources[1]) + " energy, ";
    stat += std::to_string(resources[2]) + " glass, ";
    stat += std::to_string(resources[3]) + " heat, and ";
    stat += std::to_string(resources[4]) + " WiFi.";
    return stat;
}

std::string Builder::printResourcesGained()
{
    std::string stat = "";
    std::string col = this->getName();
    int res = 0;
    std::string ress = "";

    stat += "Builder " + col + " gained:\n";

    for (int i = 0; i < 5; i++)
    {
        if (gained[i] != 0)
        {
            res = i;
            if (res == 0)
            {
                ress = " Brick";
            }
            else if (res == 1)
            {
                ress = " Energy";
            }
            else if (res == 2)
            {
                ress = " Glass";
            }
            else if (res == 3)
            {
                ress = " Heat";
            }
            else
            {
                ress = " WiFi";
            }
            stat += std::to_string(gained[res]) + ress + "\n";
        }
    }

    resetResourcesGained();
    return stat;
}

int Builder::roll(int roll)
{
    return dice.roll(roll);
}

// Building a basement function when starting game ONLY
bool Builder::startGameBasement(std::unique_ptr<Vertice> &vertice)
{
    vertice.get()->setOwner(colour);
    std::string name = this->getName();
    vertice.get()->setType(Residence::Basement);
    vertice.get()->setName(name.substr(0, 1) + "B");
    vertice.get()->attachBuilder(this);
    addPoint();
    return true;
}

// Override from Observer
std::string Builder::getName()
{
    if (colour == 0)
    {
        return "Blue";
    }
    else if (colour == 1)
    {
        return "Red";
    }
    else if (colour == 2)
    {
        return "Orange";
    }
    else
    {
        return "Yellow";
    }
}

// Override from Observer
void Builder::notify(Subject &whoNotified)
{
    //add resources
    int resource_gained; // [0]: brick, [1]: energy, [2]: glass, [3]: heat, [4]: WiFi
    if (whoNotified.getInfo().resource != Resource::Park)
    {
        if (whoNotified.getInfo().resource == Resource::Brick)
        {
            resource_gained = 0;
        }
        else if (whoNotified.getInfo().resource == Resource::Energy)
        {
            resource_gained = 1;
        }
        else if (whoNotified.getInfo().resource == Resource::Glass)
        {
            resource_gained = 2;
        }
        else if (whoNotified.getInfo().resource == Resource::Heat)
        {
            resource_gained = 3;
        }
        else
        {
            resource_gained = 4;
        }
        resources[resource_gained] += whoNotified.getInfo().numResources;
        gained[resource_gained] += whoNotified.getInfo().numResources;
    }
}

// IMPORTANT: ONLY TO BE USED WHEN LOADING BOARD FROM FILE
void Builder::setRoad(std::unique_ptr<Edge> &edge)
{
    edge.get()->buildRoad(colour);
}

// IMPORTANT: ONLY TO BE USED WHEN LOADING BOARD FROM FILE
void Builder::setResidence(std::unique_ptr<Vertice> &vertice, std::string s)
{
    vertice.get()->setOwner(colour);
    std::string name = this->getName();
    if (s == "B")
    {
        vertice.get()->setType(Residence::Basement);
        vertice.get()->setName(name.substr(0, 1) + "B");
        vertice.get()->attachBuilder(this);
        addPoint();
    }
    else if (s == "H")
    {
        vertice.get()->setType(Residence::House);
        vertice.get()->setName(name.substr(0, 1) + "H");
        vertice.get()->attachBuilder(this);
        addPoint();
        addPoint();
    }
    else if (s == "T")
    {
        vertice.get()->setType(Residence::Tower);
        vertice.get()->setName(name.substr(0, 1) + "T");
        vertice.get()->attachBuilder(this);
        addPoint();
        addPoint();
        addPoint();
    }
}

// IMPORTANT: ONLY TO BE USED WHEN LOADING BOARD FROM FILE
void Builder::setResources(int pos, int amt)
{
    resources[pos] = amt;
}

void Builder::goose() //check if there are more than 10 resources outside of function
{
    int sum = 0;
    for (int i = 0; i < 5; i++)
    { //find total resource
        sum += resources[i];
    }

    if (sum < 10)
    {
        return;
    }

    int half = floor(sum / 2); //half of resource; steal this many

    std::string resources_to_steal = ""; //string with all resources available

    for (int i = 0; i < 5; i++)
    { //loop through all resources
        for (int j = 0; j < resources[i]; j++)
        {                                            //loop through the number of each resource
            resources_to_steal += std::to_string(i); //concatinate the resource number (i.e 0 = Brick) to the string
        }
    }

    int random_resource; // = rand() % (resources_to_steal.length() + 1);
    //random number between 0 and the length of the string with resources available
    std::vector<int> stolen{0, 0, 0, 0, 0};
    //vector stores number of each resource stolen for the end
    std::string resource_stolen;
    //just a string with the single character of the resource stolen

    for (int i = 0; i < half; i++)
    {
        srand((unsigned)time(0));
        random_resource = rand() % (resources_to_steal.length());
        resource_stolen = resources_to_steal.substr(random_resource, 1);
        if (resource_stolen == "0")
        { //steal brick
            stolen[0] += 1;
            resources[0] -= 1;
        }
        else if (resource_stolen == "1")
        { //steal energy
            stolen[1] += 1;
            resources[1] -= 1;
        }
        else if (resource_stolen == "2")
        { //steal glass
            stolen[2] += 1;
            resources[2] -= 1;
        }
        else if (resource_stolen == "3")
        { //steal heat
            stolen[3] += 1;
            resources[3] -= 1;
        }
        else
        { //steal wifi
            stolen[4] += 1;
            resources[4] -= 1;
        }
        if (random_resource == resources_to_steal.length() - 1)
        {
            resources_to_steal = resources_to_steal.substr(0, random_resource);
        }
        else
        {
            resources_to_steal = resources_to_steal.substr(0, random_resource) + resources_to_steal.substr(random_resource + 1);
        }
        //remove the resource from the string of available resources
    }

    std::cout << "Builder " << getName() << " loses " << std::to_string(half) << " resources to the geese. They lost:" << std::endl;

    for (int i = 0; i < 5; i++)
    {
        if (stolen[i] != 0)
        {
            std::cout << std::to_string(stolen[i]) << " ";
            if (i == 0)
            {
                std::cout << "BRICK" << std::endl;
            }
            else if (i == 1)
            {
                std::cout << "ENERGY" << std::endl;
            }
            else if (i == 2)
            {
                std::cout << "GLASS" << std::endl;
            }
            else if (i == 3)
            {
                std::cout << "HEAT" << std::endl;
            }
            else
            {
                std::cout << "WIFI" << std::endl;
            }
        }
    }
}

std::string Builder::getResources()
{
    std::string out;
    int n = resources.size();
    for (int i = 0; i < n; i++)
    {
        out += std::to_string(resources[i]) + " ";
    }
    return out;
}

void Builder::gooseSteal(std::shared_ptr<Builder> &b)
{                                        //Builder b is the player you're stealing from
    std::string stolen = "";             //name of random stolen resource
    std::string resources_to_steal = ""; //string with all resources available

    for (int i = 0; i < 5; i++)
    { //loop through all resources
        for (int j = 0; j < b.get()->resources[i]; j++)
        {                                            //loop through the number of each resource
            resources_to_steal += std::to_string(i); //concatinate the resource number (i.e 0 = Brick) to the string
        }
    }
    srand((unsigned)time(0));
    int random_resource = rand() % (resources_to_steal.length() + 1);
    //random number between 0 and the length of the string with resources available
    std::string resource_stolen = resources_to_steal.substr(random_resource, 1);
    //just a string with the single character of the resource stolen

    if (resource_stolen == "0")
    { //steal brick
        stolen = "BRICK";
        resources[0] += 1;
        b.get()->resources[0] -= 1;
    }
    else if (resource_stolen == "1")
    { //steal energy
        stolen = "ENERGY";
        resources[1] += 1;
        b.get()->resources[1] -= 1;
    }
    else if (resource_stolen == "2")
    { //steal glass
        stolen = "GLASS";
        resources[2] += 1;
        b.get()->resources[2] -= 1;
    }
    else if (resource_stolen == "3")
    { //steal heat
        stolen = "HEAT";
        resources[3] += 1;
        b.get()->resources[3] -= 1;
    }
    else
    { //steal wifi
        stolen = "WIFI";
        resources[4] += 1;
        b.get()->resources[4] -= 1;
    }
    std::cout << "Builder " << getName() << " steals " << stolen << " from builder " << b.get()->getName() << std::endl;
}
