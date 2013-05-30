#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <string>

class CommandParser
{
    std::string command;

protected:
    CommandParser();

public:
    void run();
    CommandParser& getInstance();

};

#endif // COMMANDPARSER_H
