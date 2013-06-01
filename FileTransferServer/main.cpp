#include <QCoreApplication>
#include <QThread>
#include <QFile>
#include <QHostAddress>

#include "filetransferserver.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  QFile *file = new QFile("obraz.png");
  FileTransferServer *server = new FileTransferServer(file);
  server->execute();

  delete file;
//  serverThread.start();

  return a.exec();
}
