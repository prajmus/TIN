#include <QCoreApplication>
#include <QThread>

#include "client.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  QThread thread;
  Client::getInstance().moveToThread(&thread);
  thread.start();
  thread.connect(&a, SIGNAL(aboutToQuit()), SLOT(quit()));
  a.connect(&thread, SIGNAL(finished()), SLOT(quit()));
  thread.connect(&Client::getInstance(), SIGNAL(disconnected()), SLOT(quit()));

  Client::getInstance().connectToServer();

  return a.exec();
}
