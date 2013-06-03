#include <QCoreApplication>
#include <QThread>
#include <QFile>
#include <QHostAddress>

#include "filetransferserver.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  QFile *file = new QFile("pliktekstowy.txt");
  file->open(QIODevice::ReadOnly | QIODevice::Text);
  QTextStream qin(file);
  file->seek(0);
  while(!file->atEnd()) {
              QString line = qin.readLine();
//              qDebug() << line;
          }
  FileTransferServer *server = new FileTransferServer(file, true);
  server->execute();

  return a.exec();
}
