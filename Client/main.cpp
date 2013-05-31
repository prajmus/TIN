#include <QtCore/QCoreApplication>
#include <QDebug>
#include <QTextStream>
#include <QString>
#include <iostream>
#include "CommandParser.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CommandParser::getInstance().run();

    return a.exec();
}
