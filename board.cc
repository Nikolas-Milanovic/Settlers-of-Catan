#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <string>
#include <random>
#include <algorithm>

#include "board.h"
#include "builder.h"
#include "global.h"
#include "Info.h"

//Converts an integer that represents the builder into their respective colour
std::string Board::integerToColour(int builder)
{
	if (0 == builder)
	{
		return "Blue";
	}
	else if (1 == builder)
	{
		return "Red";
	}
	else if (2 == builder)
	{
		return "Orange";
	}
	else
	{
		return "Yellow";
	}
}

//Converts a string that represents a builder into their respective integer
int Board::stringToInteger(std::string builder)
{
	if ("Blue" == builder)
	{
		return 0;
	}
	else if ("Red" == builder)
	{
		return 1;
	}
	else if ("Orange" == builder)
	{
		return 2;
	}
	else if ("Yellow" == builder)
	{
		return 3;
	}
	else
	{
		return -1;
	}
}

//Takes in two vectors both of length 2 and find common integer
// assumes there is only one common integer
int Board::findCommonVertice(std::vector<int> v1, std::vector<int> v2)
{
	int first = v1[0];
	int second = v1[1];
	for (auto const &i : v2)
	{
		if (i == first)
		{
			return i;
		}
		if (i == second)
		{
			return i;
		}
	}
}

// Constructor for board
Board::Board()
{
	//initialize builders
	createBuilders();
}

//calls goose() in the builder class to handle players with >10 resources
void Board::gooseRolled()
{
	for (auto const &i : builders)
	{
		i->goose();
	}
}

//Function called when the player inputs a tile they want to place the goose on.
//outputs the possible builders to choose from.
bool Board::playGoose(int location)
{
	if (location < 0 || location > 18)
	{
		std::cout << "Invalid location.\n";
		return false;
	}
	int curGoose = 0;
	//finds the tile who curently has GEESE
	for (int i = 0; i < 19; i++)
	{
		if (tiles[i]->getGeese())
		{
			curGoose = i;
			break;
		}
	}
	tiles[curGoose]->setGeese(false);
	tiles[location]->setGeese(true);
	//veticesAttached is a vector of vertice locations that are on the tile @ int location
	std::vector<int> verticesAttached = vTiles[location];

	//Loops through verticesAttached and inserts (into toStealFrom set<int>) the integer value of the
	// builder who made the vertice and has atleast one resource.
	for (auto const &i : verticesAttached)
	{
		int owner = vertices[i]->getOwner();
		if (0 <= owner && builders[owner]->canSteal())
		{
			toStealFrom.insert(owner);
		}
	}

	//curBuilder can't choose them self
	if (toStealFrom.find(curBuilder->getColour()) != toStealFrom.end())
	{
		toStealFrom.erase(toStealFrom.find(curBuilder->getColour()));
	}

	std::string options;
	//Creates a string of the builders to choose from
	for (auto const &i : toStealFrom)
	{
		options = options + " " + integerToColour(i) + ",";
	}

	//remove last comma:
	options.substr(0, options.length() - 1);

	std::cout << "Builder " + integerToColour(curBuilder->getColour());

	if (0 == toStealFrom.size()) //if theres no builders to steal from:
	{
		std::cout << " has no builders to steal from." << std::endl;
		return false;
	}
	else
	{ //otherwise, print options (i.e. players to steal from)
		std::cout << " can choose from" + options << std::endl;
		return true;
	}
}

//curBuilder chooses which builder to steal from
bool Board::gooseSteal(std::string b)
{
	int builder = stringToInteger(b);
	//check for invalid builder to steal from;
	if (toStealFrom.find(builder) == toStealFrom.end())
	{
		std::cout << "Invalid choice." << std::endl;
		return false;
	}
	else
	{ //else, call gooseSteal to randomly choose one resources
		curBuilder->gooseSteal(builders[builder]);
		return true;
	}
}

// returns true if tile has goose on it
bool Board::checkgoose(int n)
{
	return tiles[n].get()->getGeese();
}

void Board::load()
{
	curBuilder.get()->setDie(Die::Loaded);
	std::cout << "Builder " << curBuilder->getName() << " now has loaded Dice." << std::endl;
}

void Board::fair()
{
	curBuilder.get()->setDie(Die::Fair);
	std::cout << "Builder " << curBuilder->getName() << " now has fair Dice." << std::endl;
}

Die Board::getDieType()
{
	return curBuilder->getDieType();
}

//roll(int rolled): handles the error checking for valid input (Die::Loaded) or producing the correct
// output (Die::Fair). Then using the observer design pattern, all tile objects notify there observers
int Board::roll(int rolled)
{
	//call rollValue to get a valid roll integer
	rolled = rollValue(rolled);
	if (!rolled)
	{ //rolled is false if a user has loaded die & doesn't input between 2 & 12
		return rolled;
	}

	//call notify observers for the tiles with the value rolled
	for (auto const &i : tiles)
	{
		if (rolled == i->getValue())
		{
			i->notifyObservers();
		}
	}
	//take care of printing resources gained
	int numOfBuildersWhoGained = 0;
	for (auto const &i : builders)
	{
		if (i->resourcesGained())
		{
			numOfBuildersWhoGained++;
			std::cout << i->printResourcesGained();
		}
	}

	if (0 == numOfBuildersWhoGained)
	{
		std::cout << "No builders gained resources." << std::endl;
	}

	return rolled;
}

//return 0 for an invlaid number between 2 and 12 (when Die::Loaded) or produces a valid roll (Die::Fair)
int Board::rollValue(int rolled)
{
	int val = curBuilder->roll(rolled);
	if (0 == val)
	{
		std::cout << "Invalid roll." << std::endl;
		return val;
	}
	else
	{
		std::cout << "You rolled a " << val << "." << std::endl;
		return val;
	}
}
// Prints the status of all builders from 0 -> 3
void Board::status()
{
	for (int i = 0; i < builders.size(); i++)
	{
		std::cout << builders[i].get()->status() << std::endl;
	}
}

//iterates through all vertices and prints the ones owned by the curBuilder
void Board::printResidence()
{
	std::cout << integerToColour(curBuilder.get()->getColour()) << " has built:" << std::endl;
	for (const auto &i : vertices)
	{
		if (i->getOwner() == curBuilder.get()->getColour())
		{
			std::cout << i->getLocation() << " " << i->getName().at(1) << std::endl;
		}
	}
}

//Checks if the road @ int r is valid:
// If theres an adjacent residence owned by curBuilder (true),
// If theres an adjacent road owned by curBuilder and the residence inbetween is owned by curBuilder (true)
bool Board::valid_road(int r)
{
	std::vector<int> adjacentVertices = adjacentVertices_Edge[r];
	std::vector<int> adjacentEdges = adjacentEdges_Edge[r];

	//First, check adjacent vertices
	for (auto const &i : adjacentVertices)
	{
		//Check if adjacentVertices (residences) are owned by the curBuilder
		if (curBuilder.get()->getColour() == vertices[i]->getOwner())
		{ //If the above condition passes, you're always allowed to build
			return true;
		}
	}
	//Second, check adjacent Edges
	for (auto const &i : adjacentEdges)
	{
		//check if adjacent Edges (roads) are owned by the curbuilder
		if (curBuilder->getColour() == edges[i]->getOwner())
		{ //If the condition passes, we must also check the residence between
			// the adjacent road and the road being built is owned by the curBuilder
			int commonVertex = findCommonVertice(adjacentVertices, adjacentVertices_Edge[i]);

			//Return true if curBuilder owns the Common Vertex or the vertex has no residence (-1)
			if (curBuilder->getColour() == vertices[commonVertex]->getOwner() ||
				-1 == vertices[commonVertex]->getOwner())
			{
				return true;
			}
		}
	}
	//Otherwise we can't build @ int r
	return false;
}

// Attemps to build Road @ road
bool Board::build_road(int road)
{
	if (road < 0 || road > 71)
	{
		std::cout << "Please enter a Road in the range 0,71" << std::endl;
		return false;
	}
	else if (edges[road].get()->getOwner() != -1)
	{
		std::cout << "This road is already owned" << std::endl;
		return false;
	}
	else if (valid_road(road) == false)
	{
		std::cout << "You can't build a road here" << std::endl;
		return false;
	}
	else if (curBuilder.get()->buildRoad(edges[road])) //calls Builder::buildRoad
	{
		std::cout << "Builder " << curBuilder->getName() << " successfully built a Road at " << road << "." << std::endl;
		return true;
	}
	return false;
}

// Attempts to improve Residence @ int res
bool Board::improve(int res)
{
	if (res < 0 || res > 53)
	{
		std::cout << "You can not build here." << std::endl;
		return false;
	}
	if (vertices[res]->getType() == Residence::NoRes)
	{
		std::cout << "No Residence has been built yet." << std::endl;
		return false;
	}
	else if (vertices[res]->getOwner() != curBuilder->getColour())
	{
		std::cout << "This Residence is owned by another builder." << std::endl;
		return false;
	}
	else if (vertices[res]->getType() == Residence::Tower)
	{
		std::cout << "Already improved to the maximum." << std::endl;
		return false;
	}
	else
	{
		return curBuilder->upgrade(vertices[res]);
	}
}

//check if the intital build (i.e. making a basement) is valid)
// The boolean beginning_res is for the beginning of the game when you get 2 free builds
// as in that cases you don't need any adjacent roads owned by the curBuilder
bool Board::valid_Basement(int b, bool beginning_res)
{
	// checks if that vertice already has an owner
	if (vertices[b].get()->getOwner() != -1)
	{
		std::cout << "There is a residence at this vertex already" << std::endl;
		return false;
	}

	// checks if adjacent vertices have an owner
	for (auto i : adjacentVertices_Vertice[b])
	{
		if (vertices[i]->getOwner() != -1)
		{
			std::cout << "You cannot build adjacent to another residence." << std::endl;
			return false;
		}
	}

	//additional check: The curBuilder owns a road connected (adjacent) to vertex @ b
	if (!beginning_res)
	{
		for (auto i : adjacentEdges_Vertice[b])
		{
			if (edges[i]->getOwner() == curBuilder->getColour())
			{
				std::cout << "Builder " << curBuilder->getName() << " successfully built a Basement at " << b << "." << std::endl;
				return true;
			}
		}
		std::cout << "There are no adjacent roads built by you" << std::endl;
		return false;
	}
	return true;
}

// Attempts to build a res @ int res
// boolean beginning_res is true at the beginning of the game when you get 2 free builds
bool Board::build_res(int res, bool beginning_res)
{
	if (res < 0 || res > 53)
	{
		std::cout << "You can not build here." << std::endl;
		return false;
	}
	else if (!valid_Basement(res, beginning_res))
	{
		return false;
	}
	else if (beginning_res)
	{
		return curBuilder->startGameBasement(vertices[res]);
	}
	else
	{
		return curBuilder->buildBasement(vertices[res]);
	}
}

//Checks that user has inputed a valid trade statement
bool Board::tradeLimitations(std::string tradeBuilder, std::string give, std::string take)
{

	if (-1 == stringToInteger(tradeBuilder))
	{ //-1 represents that the string didn't match any builder colour
		std::cout << "Invalid builder name." << std::endl;
		return false;
	}
	if (-1 == stringToInt(give) || -1 == stringToInt(take))
	{
		std::cout << "Invalid resource" << std::endl;
		return false;
	}

	if (getCurColour() == tradeBuilder)
	{
		std::cout << "Why do you want to trade with yourself?" << std::endl;
		return false;
	}
	if (give == take)
	{
		std::cout << "Why do you want to trade for the same Resource?" << std::endl;
		return false;
	}
	return true;
}

//Attempts trade between curBuilder and string trader & assumes Board::tradeLimitations is true
bool Board::trade(std::string trader, std::string give, std::string take)
{
	int toTradeWith = stringToInteger(trader);
	return curBuilder.get()->trade(builders[toTradeWith], give, take);
}

// Goes to the next builder
void Board::next(bool print)
{
	if (0 == curBuilder.get()->getColour())
	{
		builders[0] = curBuilder;
		curBuilder = builders[1];
	}
	else if (1 == curBuilder.get()->getColour())
	{
		builders[1] = curBuilder;
		curBuilder = builders[2];
	}
	else if (2 == curBuilder.get()->getColour())
	{
		builders[2] = curBuilder;
		curBuilder = builders[3];
	}
	else
	{
		builders[3] = curBuilder;
		curBuilder = builders[0];
	}
	if (print)
	{
		std::cout << std::string("Builder ") + integerToColour(curBuilder.get()->getColour()) + "'s turn.\n> ";
	}
}
// Goes to prev Builder
void Board::prev(bool print)
{
	if (0 == curBuilder.get()->getColour())
	{
		builders[0] = curBuilder;
		curBuilder = builders[3];
	}
	else if (1 == curBuilder.get()->getColour())
	{
		builders[1] = curBuilder;
		curBuilder = builders[0];
	}
	else if (2 == curBuilder.get()->getColour())
	{
		builders[2] = curBuilder;
		curBuilder = builders[1];
	}
	else
	{
		builders[3] = curBuilder;
		curBuilder = builders[2];
	}
	if (print)
	{
		std::cout << std::string("Builder ") + integerToColour(curBuilder.get()->getColour()) + "'s turn.\n> ";
	}
}

std::string Board::getCurColour()
{
	return integerToColour(curBuilder.get()->getColour());
}

//Check if the current player wins
bool Board::win()
{
	bool doesWin = (curBuilder.get()->getPoints() >= 10);
	if (doesWin)
	{
		std::cout << "Player: " << integerToColour(curBuilder.get()->getColour()) << " has won" << std::endl;
	}
	return doesWin;
}

void Board::setBuilder(int b, std::string line)
{
	std::string s;
	std::string s2;
	std::stringstream str_strm(line);
	int pos = 0;
	// sets resources of builder
	while (str_strm >> s)
	{
		if (s == "r")
		{
			break;
		}
		builders[b].get()->setResources(pos, std::stoi(s));
		pos += 1;
	}
	// sets roads of builder
	while (str_strm >> s)
	{
		if (s == "h")
		{
			break;
		}

		builders[b].get()->setRoad(edges[std::stoi(s)]);
	}
	// sets residences of builder
	while (str_strm >> s)
	{
		str_strm >> s2;
		builders[b].get()->setResidence(vertices[std::stoi(s)], s2);
	}
}

// Inializes the board based on saved file
void Board::loadFromSave(std::string save)
{
	std::ifstream boardFromSave(save);
	std::vector<std::string> file;
	if (boardFromSave.is_open())
	{
		// reads in all the lines of the file
		std::string line;
		while (std::getline(boardFromSave, line))
		{
			file.emplace_back(line);
		}
		boardFromSave.close();
	}
	else
	{
		throw std::invalid_argument("ERROR: Unable to open file " + save + " to load game.\n");
	}
	// reads line6 and sets board layout
	int resource;
	int pos = 0;
	int val;
	std::string word;
	std::string word2;
	std::stringstream stringstrm(file[5]);
	while (stringstrm >> word)
	{
		stringstrm >> word2;
		resource = std::stoi(word);
		val = std::stoi(word2);
		Resource r;
		r = intToResource(resource);

		auto tile = std::make_unique<Tile>(pos, r, val);
		tiles.push_back(std::move(tile));
		pos += 1;
	}
	// create Vertices & Edges, attach Vertices to tiles
	createVE();
	attachVE();

	// Sets builder resources, residences, and roads
	setBuilder(0, file[1]);
	setBuilder(1, file[2]);
	setBuilder(2, file[3]);
	setBuilder(3, file[4]);

	// reads last line, sets goose to tile specified
	tiles[std::stoi(file[6])].get()->setGeese(true);

	// reads first line, sets current builder to it
	curBuilder = builders[std::stoi(file[0])];

	// prints board
	print();
	// prints curBuilder
	std::cout << std::string("Builder ") + integerToColour(curBuilder.get()->getColour()) + "'s turn.\n> ";
}
// initializes the board based on values read in
void Board::loadFromFile(std::string file)
{
	std::ifstream boardfromFile(file);
	if (boardfromFile.is_open())
	{
		std::string word;
		std::string word2;
		int resource;
		int val;
		int pos = 0;
		while (boardfromFile >> word)
		{
			// reads in words
			boardfromFile >> word2;
			resource = std::stoi(word);
			val = std::stoi(word2);
			Resource r;
			r = intToResource(resource);
			// sets tile to those values
			auto tile = std::make_unique<Tile>(pos, r, val);
			// sets goose to park tile
			if (r == Resource::Park)
			{
				tile.get()->setGeese(true);
			}
			tiles.push_back(std::move(tile));
			pos += 1;
		}
	}
	else
	{
		throw std::invalid_argument("ERROR: Unable to open file " + file + " for board layout.\n");
	}
	createVE();
	attachVE();
}

// initializes the board based on random generated values
void Board::loadFromRandom(unsigned int seed)
{
	// Need to follow distribution set out in tile distribution
	std::default_random_engine rng{seed};
	// need to allocate park tile and
	// will have to add goose to one tile, do that first and block the tile off from random order
	std::vector<int> resources = {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5};
	// 2, 12, 2x(3, 4, 5, 6, 8, 9, 10, 11) plus 1x7 (Park).
	std::vector<int> values = {2, 12, 3, 4, 5, 6, 8, 9, 10, 11, 3, 4, 5, 6, 8, 9, 10, 11};

	for (int i = 0; i < 19; i++)
	{
		Resource r;
		std::shuffle(resources.begin(), resources.end(), rng);
		std::shuffle(values.begin(), values.end(), rng);

		int resource = resources.back();
		int value;
		if (resource == 5)
		{
			r = Resource::Park;
			value = 7;
			auto tile = std::make_unique<Tile>(i, r, value);
			tile.get()->setGeese(true);
			tiles.push_back(std::move(tile));
			resources.pop_back();
		}
		else
		{

			r = intToResource(resource);
			value = values.back();
			auto tile = std::make_unique<Tile>(i, r, value);
			tiles.push_back(std::move(tile));
			values.pop_back();
			resources.pop_back();
		}
	}
	createVE();
	attachVE();
}

// Creates Vertice & Edge Vectors
void Board::createVE()
{
	for (int i = 0; i < 54; i++)
	{
		auto vertice = std::make_unique<Vertice>(i);
		vertices.push_back(std::move(vertice));
	}
	for (int i = 0; i < 72; i++)
	{
		auto edge = std::make_unique<Edge>(i);
		edges.push_back(std::move(edge));
	}
}
// attaches vertices to tiles
void Board::attachVE()
{
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < vTiles[i].size(); j++)
		{
			tiles[i].get()->attach(vertices[vTiles[i][j]].get());
		}
	}
}
// Prints the board to standard out
void Board::print()
{
	// Reads from a formatted board .txt file
	std::ifstream boardFromtext("board.txt");
	std::string line;

	if (boardFromtext.is_open())
	{
		// Counters (we need to retain therefore outside of for loop)
		// to print in order
		int i = 0;
		int j = 0;
		int k = 0;
		int l = 0;
		int n = 0;
		int g = 0;
		while (std::getline(boardFromtext, line))
		{
			int pos = 0;
			// Replaces all instances of XX with vertice value
			pos = line.find("XX");
			while (pos != std::string::npos)
			{
				line.replace(pos, 2, vertices[i].get()->print());
				pos = line.find("XX");
				i += 1;
			}
			// Replaces all instances of XY with edge value
			pos = line.find("XY");
			while (pos != std::string::npos)
			{
				line.replace(pos, 2, edges[j].get()->print());
				pos = line.find("XY");
				j += 1;
			}
			// Replaces all instances of TP with tile position
			pos = line.find("TP");
			while (pos != std::string::npos)
			{
				line.replace(pos, 2, tiles[k].get()->printLocation());
				pos = line.find("TP");
				k += 1;
			}
			// Replaces all instances of TT with tile type
			pos = line.find("TT");
			while (pos != std::string::npos)
			{
				line.replace(pos, 2, tiles[n].get()->printResource());
				pos = line.find("TT");
				n += 1;
			}
			// Replaces all instances of RV with resource value
			pos = line.find("RV");
			while (pos != std::string::npos)
			{
				line.replace(pos, 2, tiles[l].get()->printValue());
				pos = line.find("RV");
				l += 1;
			}
			// Replace all instances of Gs with goose value
			pos = line.find("GS");
			while (pos != std::string::npos)
			{
				line.replace(pos, 2, tiles[g].get()->printGeese());
				pos = line.find("GS");
				g += 1;
			}
			std::cout << line << std::endl;
		}
		boardFromtext.close();
	}
	else
	{
		std::cout << "Error, could not open Board.txt" << std::endl;
	}
}
//note: save doesn't save num of points per player!!!

void Board::save(std::string file)
{
	std::ofstream os{file};
	//<curTurn>
	os << curBuilder->getColour() << std::endl;
	//<builder(0..3)Data>
	//concatenate resources
	std::string builder0 = builders[0]->getResources() + "r ";
	std::string builder1 = builders[1]->getResources() + "r ";
	std::string builder2 = builders[2]->getResources() + "r ";
	std::string builder3 = builders[3]->getResources() + "r ";

	//concatenate roads
	for (auto const &i : edges)
	{
		if (0 == i->getOwner())
		{
			builder0 += std::to_string(i->getLocation()) + " ";
		}
		else if (1 == i->getOwner())
		{
			builder1 += std::to_string(i->getLocation()) + " ";
		}
		else if (2 == i->getOwner())
		{
			builder2 += std::to_string(i->getLocation()) + " ";
		}
		else if (3 == i->getOwner())
		{
			builder3 += std::to_string(i->getLocation()) + " ";
		}
	}
	builder0 += "h ";
	builder1 += "h ";
	builder2 += "h ";
	builder3 += "h ";

	//concatenate housing
	std::string name;
	for (auto const &i : vertices)
	{
		name = i->getName();
		if (0 == i->getOwner())
		{
			builder0 += std::to_string(i->getLocation()) + " " + name.substr(1) + " ";
		}
		else if (1 == i->getOwner())
		{
			builder1 += std::to_string(i->getLocation()) + " " + name.substr(1) + " ";
		}
		else if (2 == i->getOwner())
		{
			builder2 += std::to_string(i->getLocation()) + " " + name.substr(1) + " ";
		}
		else if (3 == i->getOwner())
		{
			builder3 += std::to_string(i->getLocation()) + " " + name.substr(1) + " ";
		}
	}
	os << builder0 << std::endl;
	os << builder1 << std::endl;
	os << builder2 << std::endl;
	os << builder3 << std::endl;

	//<board>
	int geese;
	int count = 0;
	for (auto const &i : tiles)
	{
		if (true == i->getGeese())
		{
			geese = count;
		}
		Resource r = i->getInfo().resource;
		os << resourceToInteger(r) << " " << i->getValue() << " ";
		count++;
	}
	os << std::endl;

	//<geese>
	os << geese << std::endl;
}

// creates builders
void Board::createBuilders()
{
	for (int i = 0; i < 4; i++)
	{
		auto builder = std::make_shared<Builder>(i);
		builders.push_back(std::move(builder));
	}
	curBuilder = builders[0];
}

// resets board
void Board::reset()
{
	builders.clear();
	vertices.clear();
	edges.clear();
	tiles.clear();
	toStealFrom.clear();
}
