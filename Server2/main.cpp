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

    CommunicationServer::getInstance().execute();
    //FileServer::getInstance().viewFileList();




    return a.exec();
}
