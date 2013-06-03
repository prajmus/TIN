#include "communicationserver.h"

CommunicationServer::CommunicationServer(QObject *parent) : m_state(IDLE)
{
  m_parentThread = QThread::currentThread();

  connect(&m_serverThread, SIGNAL(started()), this, SLOT(startListening()));
}

CommunicationServer::~CommunicationServer()
{
  if(m_state != IDLE) {
    qDebug() << "Still running";
    disconnectSlot();
  }
  delete m_server;
}

void CommunicationServer::execute()
{
  moveToThread(&m_serverThread);
  m_serverThread.moveToThread(&m_serverThread);
  m_serverThread.start();
}

void CommunicationServer::startListening()
{
  m_server = new QTcpServer();
  m_clientList = new QList<QTcpSocket* >();
  connect(m_server, SIGNAL(newConnection()), this, SLOT(addNewClient()));
  if(m_server->listen(QHostAddress::Any, PORT)) {
    m_state = LISTENING;
    return;
  }
  exit(1);
}

void CommunicationServer::disconnectSlot()
{
  QTcpSocket *socket = qobject_cast<QTcpSocket*>(QObject::sender());

  if (!socket)
        return;
  m_clientList->removeAll(socket);
  socket->deleteLater();
//  moveToThread(m_parentThread);
//  m_serverThread.moveToThread(m_parentThread);
//  m_server->close();
//  m_serverThread.quit();
}

void CommunicationServer::threadFinished()
{
  m_state = IDLE;
  this->deleteLater();
}

void CommunicationServer::addNewClient()
{
  QTcpSocket *client;
  while(m_server->hasPendingConnections()) {
    client = m_server->nextPendingConnection();
    connect(client, SIGNAL(disonnected()), this, SLOT(disconnectSlot()));
    m_clientList->append(client);
    m_state = CONNECTED;
    connect(&m_serverThread, SIGNAL(finished()), this, SLOT(threadFinished()));
  }
}
