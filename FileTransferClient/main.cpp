#include <QCoreApplication>
#include <QThread>

#include "client.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  QFile *file= new QFile("wallpaper.jpg");
  Client *client = new Client(6000, file, false);
  client->execute();

  return a.exec();
}
