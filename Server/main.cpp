#include <QCoreApplication>
#include <cassert>
#include <iostream>
#include "fileserver.h"
#include <QStringList>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    FileServer::getInstance().addFileToList("/usr/cos/a.txt", 1);
    FileServer::getInstance().addFileToList("/usr/cos/b.txt", 1);
    FileServer::getInstance().addFileToList("/usr/cos/c.txt", 1);
    FileServer::getInstance().removeFileFromList("/usr/cos/a.txt");

    //FileServer::getInstance().viewFileList();






//    AccountBase::getInstance();
//    QString str = QString::fromStdString(md5("test"));
//    Database::getInstance().dbOpen();

//    qDebug() << AccountBase::getInstance().registerUser(QString("a"), str);
//    qDebug() << AccountBase::getInstance().registerUser(QString("b"), str);

//    qDebug() << AccountBase::getInstance().exists(1);
//    qDebug() << AccountBase::getInstance().exists(2);
//    qDebug() << AccountBase::getInstance().exists(3);
//    qDebug() << AccountBase::getInstance().exists(4);
//    qDebug() << AccountBase::getInstance().exists(5);
//    qDebug() << AccountBase::getInstance().exists(6);
//    qDebug() << AccountBase::getInstance().exists(7);

//    qDebug() << AccountBase::getInstance().deleteUser(1);

//    qDebug() << AccountBase::getInstance().exists(1);

//    qDebug() << AccountBase::getInstance().registerUser(QString("a"), str);
//    qDebug() << AccountBase::getInstance().exists(1);
//    qDebug() << AccountBase::getInstance().exists(2);
//    qDebug() << AccountBase::getInstance().exists(3);
    return a.exec();
}
