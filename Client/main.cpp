#include <QtCore/QCoreApplication>
#include <QDebug>
#include <QTextStream>
#include <QString>
#include <iostream>
#include "client.h"
#include "fileserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //Client::getInstance().run();

    FileServer* serwerek = new FileServer("/home/qiubix/TIN/Client/");

    return a.exec();
}
