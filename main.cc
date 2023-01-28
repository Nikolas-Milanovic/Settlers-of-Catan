#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <stdexcept>
#include <limits>
#include <fstream>
#include "board.h"

// Gets integer input, handles string/type errors errors
int getInt(std::istringstream &iss)
{
    int pos;
    if (!(iss >> pos))
    {
        std::cout << "Please enter a valid integer" << std::endl
                  << "> ";
    }
    else
    {
        return pos;
    }
    while (!(std::cin >> pos))
    {
        std::cout << "Please enter a valid integer" << std::endl
                  << "> ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return pos;
}

int getInt()
{
    int pos;
    while (!(std::cin >> pos))
    {
        std::cout << "Please enter a valid integer" << std::endl
                  << "> ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return pos;
}

//Converts the string to have teh first letter a captial and the rest lower case. HELLO->Hello
void convert(std::string &s)
{
    int len = s.length();

    if (len > 0)
    {
        s[0] = toupper(s[0]);
    }

    for (int i = 1; i < len; i++)
    {
        s[i] = tolower(s[i]);
    }
}

bool startGame(Board &board, bool start)
{
    std::string s;
    bool beginning = true;
    try
    {
        if (start)
        {
            for (int i = 0; i < 4; i++)
            {
                std::cout << std::string("Builder ") + board.getCurColour() + ", where do you want to build a basement?\n> ";
                int n = getInt();
                while (!board.build_res(n, true))
                {
                    n = getInt();
                }
                if (i != 3)
                {
                    board.next(false);
                }
            }
            for (int i = 0; i < 4; i++)
            {
                std::cout << std::string("Builder ") + board.getCurColour() + ", where do you want to build a basement?\n> ";
                int n = getInt();
                while (!board.build_res(n, true))
                {
                    n = getInt();
                }
                if (i != 3)
                {
                    board.prev(false);
                }
            }
            start = false;
            board.print();
            std::cout << std::string("Builder ") + board.getCurColour() + "'s turn.\n";
        }

        while (std::getline(std::cin, s))
        {
            // start of Game input handler
            int n;
            std::istringstream iss{s};
            std::string cmd;
            iss >> cmd;

            // Beginning of Turn Input Handler
            if (beginning)
            {
                if (cmd == "load")
                {
                    board.load();
                }
                else if (cmd == "fair")
                {
                    board.fair();
                }
                else if (cmd == "roll")
                {
                    int roll;
                    if (Die::Loaded == board.getDieType())
                    {
                        do
                        {
                            std::cout << "Input a roll between 2 and 12:" << std::endl;
                            std::cout << "> ";
                            int n = getInt();
                            roll = board.roll(n);
                        } while (!roll);
                        beginning = false;
                    }
                    else
                    {
                        roll = board.roll();
                        beginning = false;
                    }

                    if (7 == roll)
                    { //GEESE
                        board.gooseRolled();

                        int n;
                        while (true)
                        {
                            std::cout << "Choose where to place the GEESE.\n> ";
                            n = getInt();
                            if (n < 0 || n > 18)
                            {
                                std::cout << "You can not move geese there" << std::endl;
                            }
                            else if (board.checkgoose(n))
                            {
                                std::cout << "You can not move geese there" << std::endl;
                            }
                            else
                            {
                                break;
                            }
                        }

                        if (board.playGoose(n))
                        {
                            std::string response;
                            do
                            {
                                std::cout << "Choose a builder to steal from.\n> ";
                                std::cin >> response;
                                convert(response);
                            } while (!board.gooseSteal(response));
                        }
                    }
                }
                else if (cmd == "help")
                {
                    std::cout << "Valid commands:" << std::endl;
                    std::cout << "load" << std::endl;
                    std::cout << "fair" << std::endl;
                    std::cout << "roll" << std::endl;
                }
                else if (cmd != "")
                {
                    std::cerr << "Invalid Command." << std::endl;
                    std::cout << "Please enter 'help' for a list of valid commands." << std::endl;
                }
                if (beginning)
                {
                    std::cout << "> ";
                }
            }
            // During the Turn Input Handler
            else
            {
                if (cmd == "board")
                {
                    board.print();
                }
                else if (cmd == "status")
                {
                    board.status();
                }
                else if (cmd == "residences")
                {
                    board.printResidence();
                }
                else if (cmd == "build-road")
                {
                    int n = getInt(iss);
                    board.build_road(n);
                }
                else if (cmd == "build-res")
                {
                    int n = getInt(iss);
                    board.build_res(n, false);
                    if (board.win())
                    {
                        std::string response;
                        // Prompt to play again
                        std::cout << "Would you like to play again?" << std::endl;
                        std::cin >> response;
                        convert(response);
                        if (response == "Yes")
                        {
                            return true;
                        }
                        return false;
                    }
                }
                else if (cmd == "improve")
                {
                    int n = getInt(iss);
                    board.improve(n);
                    if (board.win())
                    {
                        std::string response;
                        // Prompt to play again
                        std::cout << "Would you like to play again?" << std::endl;
                        std::cin >> response;
                        if (response == "yes")
                        {
                            return true;
                        }
                        return false;
                    }
                }
                else if (cmd == "trade")
                {
                    std::string colour;
                    std::string give;
                    std::string take;
                    std::string response;
                    iss >> colour;
                    iss >> give;
                    iss >> take;
                    convert(colour);
                    convert(give);
                    convert(take);
                    if (board.tradeLimitations(colour, give, take))
                    {
                        std::cout << "> > > " << board.getCurColour() << " offers " + colour + " one " + give + " for one " + take + "." << std::endl;
                        std::cout << "Does " + colour + " accept this offer? (yes/no)" << std::endl;
                        std::cin >> response;
                        convert(response);
                        if ("Yes" == response)
                        {
                            board.trade(colour, give, take);
                        }
                    }
                }
                else if (cmd == "next")
                {
                    board.print();
                    board.next(true);
                    beginning = true;
                }
                else if (cmd == "save")
                {
                    std::string file;
                    iss >> file;
                    board.save(file);
                }
                else if (cmd == "help")
                {
                    std::cout << "Valid commands:" << std::endl;
                    std::cout << "board" << std::endl;
                    std::cout << "status" << std::endl;
                    std::cout << "residences" << std::endl;
                    std::cout << "build-road <edge#>" << std::endl;
                    std::cout << "build-res <housing#>" << std::endl;
                    std::cout << "improve <housing#>" << std::endl;
                    std::cout << "trade <colour> <give> <take>" << std::endl;
                    std::cout << "next" << std::endl;
                    std::cout << "save <file>" << std::endl;
                    std::cout << "help" << std::endl;
                }
                else if (cmd != "")
                {
                    std::cerr << "Invalid Command." << std::endl;
                    std::cout << "Please enter 'help' for a list of valid commands." << std::endl;
                }
                if (!beginning && cmd != "trade")
                {
                    std::cout << "Enter a command:" << std::endl;
                    std::cout << "> ";
                }
            }
        }
    }
    // saves to backup.sv & exits
    catch (std::ios::failure &)
    {
        std::cout << "End of file reached." << std::endl;
        std::cout << "Saving to backup.sv..." << std::endl;
        board.save("backup.sv");
        board.reset();
        return false;
    }
}

int main(int argc, char **argv)
{
    Board board;
    bool readLoad = false;
    bool readBoard = false;
    bool readRandom = false;
    bool startofGame = true;
    int pos = 0;
    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
    // sets exceptions
    std::cin.exceptions(std::ios::eofbit);
    // Harness to read in command line arguments
    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "-seed")
        {
            try
            {
                seed = std::stoi(argv[i + 1]);
            }
            catch (std::invalid_argument &e)
            {
                std::cout << "Invalid seed" << std::endl;
                return 1;
            }
            catch (std::out_of_range &e)
            {
                std::cout << "Invalid seed" << std::endl;
                return 1;
            }
        }
        else if (arg == "-load")
        {
            readLoad = true;
            pos = i + 1;
        }
        else if (arg == "-board")
        {
            readBoard = true;
            pos = i + 1;
        }
        else if (arg == "-random-board")
        {
            readRandom = true;
        }
    }
    if (readLoad && readBoard)
    {
        std::cout << "already specified -board, can't also specify -load" << std::endl;
        return 1;
    }
    else if (readLoad && readRandom)
    {
        std::cout << "Ignoring -random-board in favour of -load" << std::endl;
        try
        {
            std::string file = argv[pos];
            board.loadFromSave(file);
            startofGame = false;
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << e.what();
            return 1;
        }
    }
    else if (readBoard && readRandom)
    {
        std::cout << "Ignoring -random-board in favour of -board" << std::endl;
        std::string file = argv[pos];
        try
        {
            board.loadFromFile(file);
            // print board before starting for loop
            board.print();
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << e.what();
            return 1;
        }
    }
    // default behaviour to read from layout.txt if no CLI specified
    else if (!readLoad && !readRandom && !readBoard)
    {
        try
        {
            board.loadFromFile("layout.txt");
            // print board before starting for loop
            board.print();
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << "Unable to open file layout.txt for default board layout." << std::endl;
            return 1;
        }
    }
    else
    {
        if (readLoad)
        {
            try
            {
                std::string file = argv[pos];
                board.loadFromSave(file);
                startofGame = false;
            }
            catch (const std::invalid_argument &e)
            {
                std::cerr << e.what();
                return 1;
            }
        }
        else if (readBoard)
        {
            std::string file = argv[pos];
            try
            {
                board.loadFromFile(file);
                // print board before starting for loop
                board.print();
            }
            catch (const std::invalid_argument &e)
            {
                std::cerr << e.what();
                return 1;
            }
        }
        else if (readRandom)
        {
            try
            {
                board.loadFromRandom(seed);
                // print board before starting for loop
                board.print();
            }
            catch (const std::invalid_argument &e)
            {
            }
        }
    }

    while (startGame(board, startofGame))
    {
        // reset board & generate a random board based on new seed
        seed = std::chrono::system_clock::now().time_since_epoch().count();
        // wipes board
        board.reset();
        // creates builders
        board.createBuilders();
        // creates random board(tiles, vertices, edges) from seed
        board.loadFromRandom(seed);
        // allows players to pick basements again
        startofGame = true;
        // prints board once
        board.print();
    }

    // before exiting clear board
    board.reset();
    // clear the board, free memory return 0;
}
