#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <QString>

class CommandParser
{
    QString command;

protected:
    CommandParser();

public:
    void run();
    static CommandParser& getInstance();

};

#endif // COMMANDPARSER_H
