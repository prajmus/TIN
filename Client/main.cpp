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

    Client::getInstance().run();

//    FileServer::getInstance().construct("/home/qiubix/TIN/Client/");
//    FileServer::getInstance().addFileToList("/home/qiubix/TIN/Client/.");

    return a.exec();
}
