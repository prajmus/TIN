#include "filetransferserver.h"
#include <QEventLoop>
#include <QTimer>
#include <QDebug>
#include <QFile>


FileTransferServer::FileTransferServer(QFile *file, QObject *parent) : m_file(file)
{
  parentThread = QThread::currentThread();

  connect(&serverThread, SIGNAL(started()), this, SLOT(startListening()));
}

void FileTransferServer::stop()
{
  serverThread.quit();
}

void FileTransferServer::execute()
{
  moveToThread(&serverThread);
  serverThread.moveToThread(&serverThread);

  serverThread.start();
}

void FileTransferServer::startListening()
{
  server = new QTcpServer();
  connect(server, SIGNAL(newConnection()), this, SLOT(addNewClient()));
  for (quint16 i = MIN_PORT; i<MAX_PORT; ++i) {
    if (server->listen(QHostAddress::Any, i)) {
      qDebug() << i;
      return;
    }
    else
      qDebug() << "Cannot bind port " << i;
  }
  exit(1);
}


void FileTransferServer::timerAction()
{
}

void FileTransferServer::addNewClient()
{
  QTcpSocket *client;
  if(server->hasPendingConnections()) {
    client = server->nextPendingConnection();
    socket = new ServerClient(client);
    socket->testFile();
  }
}


FileTransferServer::~FileTransferServer()
{

}
