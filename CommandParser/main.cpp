#include <QtCore/QCoreApplication>

#include "CommandParser.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    CommandParser &cp = CommandParser::getInstance();

    cp.run();

    return a.exec();
}
