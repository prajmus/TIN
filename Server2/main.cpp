#include <QCoreApplication>
#include <iostream>
#include <QStringList>
#include <QDebug>

#include <cassert>

#include "fileserver.h"
#include "communicationserver.h"
#include "accountserver.h"
#include "database.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    FileServer::getInstance();
    CommunicationServer::getInstance().execute();
    //FileServer::getInstance().viewFileList();
//    AccountServer::getInstance();
//    Database::getInstance().create();



    return a.exec();
}
