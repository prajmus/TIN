#include "filetransferserver.h"
#include <QEventLoop>
#include <QTimer>
#include <QDebug>
#include <QFile>


FileTransferServer::FileTransferServer(QFile *file, bool isSender, QObject *parent) : m_file(file),
  m_state(IDLE), m_sender(isSender)
{
  m_parentThread = QThread::currentThread();

  connect(&m_serverThread, SIGNAL(started()), this, SLOT(startListening()));
}

FileTransferServer::~FileTransferServer()
{
  if (m_state != IDLE) {
    qDebug() << "Still runnin'";
    disconnectSlot();
  }
  delete m_server;
}

void FileTransferServer::execute()
{
  moveToThread(&m_serverThread);
  m_serverThread.moveToThread(&m_serverThread);
  m_serverThread.start();
}

void FileTransferServer::startListening()
{
  m_server = new QTcpServer();
  connect(m_server, SIGNAL(newConnection()), this, SLOT(addNewClient()));
  for (quint16 i = MIN_PORT; i<MAX_PORT; ++i) {
    if (m_server->listen(QHostAddress::Any, i)) {
      qDebug() << i;
      m_state = LISTENING;
      return;
    }
    else
      qDebug() << "Cannot bind port " << i;
  }
  exit(1);
}

void FileTransferServer::disconnectSlot()
{
  moveToThread(m_parentThread);
  m_serverThread.moveToThread(m_parentThread);
  m_server->close();
  delete m_socket;
  m_serverThread.quit();
}

void FileTransferServer::threadFinished()
{
  m_state = IDLE;
  delete m_file;
  this->deleteLater();
}

void FileTransferServer::addNewClient()
{
  QTcpSocket *client;
  if(m_server->hasPendingConnections()) {
    client = m_server->nextPendingConnection();
    m_socket = new ServerClient(client, m_file, m_sender);
    m_state = CONNECTED;
    connect(&m_serverThread, SIGNAL(finished()), this, SLOT(threadFinished()));
    m_socket->sendInit();
    if (m_sender == true)
      m_socket->sendFile();
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(disconnectSlot()));
  }
}


