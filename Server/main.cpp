#include <QCoreApplication>
#include <cassert>
#include <iostream>
#include "fileserver.h"
#include <QStringList>
#include <QDebug>
#include "accountserver.h"
#include "database.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //qDebug() << FileServer::getInstance().addFileToList("a.txt", 1);
    qDebug() << FileServer::getInstance().addFileToList("b.txt", 1);
    //qDebug() << FileServer::getInstance().listFiles();
    //qDebug() << FileServer::getInstance().removeFile("a.txt");
    return a.exec();
}
