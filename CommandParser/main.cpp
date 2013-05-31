#include <QtCore/QCoreApplication>

#include "CommandParser.h"

#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::cout << "main()\n";
    
    CommandParser::getInstance().run();

    //std::cout << "koniec run'a\n";

    return a.exec();
}
