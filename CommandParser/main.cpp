#include <QtCore/QCoreApplication>

#include "CommandParser.h"

#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    CommandParser::getInstance().run();

    return a.exec();
}
