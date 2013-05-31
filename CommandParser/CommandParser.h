#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <QString>
#include <string>

class CommandParser
{
    std::string command1;
    QString command;

protected:
    CommandParser();

public:
    void run();
    static CommandParser& getInstance();

};

#endif // COMMANDPARSER_H
