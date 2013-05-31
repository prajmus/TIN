#include <QCoreApplication>
#include <QThread>
#include <QHostAddress>

#include "filetransferserver.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

//  QThread serverThread;
  FileTransferServer::getInstance();//.moveToThread(&serverThread);
//  serverThread.start();
//  serverThread.connect(&a, SIGNAL(aboutToQuit()), SLOT(quit()));
//  FileTransferServer::getInstance().connect(&serverThread, SIGNAL(started()), SLOT(execute()));
////  FileTransferServer::getInstance().execute();

//  serverThread.start();

  return a.exec();
}
