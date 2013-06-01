#include <QCoreApplication>
#include <QThread>
#include <QFile>
#include <QHostAddress>

#include "filetransferserver.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  QFile *file = new QFile("wallpaper.jpg");
  FileTransferServer *server = new FileTransferServer(file, true);
  server->execute();

  return a.exec();
}
