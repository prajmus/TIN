#include "filetransferserver.h"
#include <QEventLoop>
#include <QTimer>
#include <QDebug>
#include <QFile>


FileTransferServer::FileTransferServer(QObject *parent)
{
//  timer = new QTimer(this);
//  connect(timer, SIGNAL(timeout()), this, SLOT(timerAction()));
//  timer->start(10000);
  connect(&server, SIGNAL(newConnection()), this, SLOT(addNewClient()));

//
  execute();
}

FileTransferServer& FileTransferServer::getInstance()
{
  static FileTransferServer instance;
  return instance;
}

void FileTransferServer::stop()
{
  serverThread.quit();
  qDeleteAll(clients);
  clients.clear();
}

void FileTransferServer::execute()
{
  if(!server.listen(QHostAddress::Any, PORT_NUMBER))
  {
    qDebug() << "Cannot bind port";
    return;
  }
}

void FileTransferServer::timerAction()
{
}

void FileTransferServer::addNewClient()
{
  QTcpSocket *client;
  while(server.hasPendingConnections()) {
    client = server.nextPendingConnection();
    ServerClient *s = new ServerClient(client);
    s->testFile();
    clients.append(s);
  }
}
