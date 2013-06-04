#include "filetransferserver.h"
#include <QEventLoop>
#include <QTimer>
#include <QDebug>
#include <QFile>

#include "messagequeue.h"


FileTransferServer::FileTransferServer(QString file, bool isSender, QTcpSocket* withWho, QObject *parent) :
  m_fileName(file), m_state(IDLE), m_sender(isSender), m_withWho(withWho)
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
  qDebug() << "file off";
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
      m_port = i;
      m_file = new QFile(m_fileName);
      emit listening();
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
  MessageQueue::getInstance().transferFinished(m_fileName, m_sender, m_withWho);
  m_state = IDLE;
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


quint16 FileTransferServer::getPort()
{
  return m_port;
}

