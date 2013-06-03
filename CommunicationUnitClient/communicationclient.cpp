#include "communicationclient.h"

#include <QHostAddress>
#include <QDataStream>

CommunicationClient::CommunicationClient(QObject *parent) :
  QObject(parent), m_state(IDLE)
{
  m_parentThread = QThread::currentThread();
  connect(&m_clientThread, SIGNAL(started()), this, SLOT(connectToServer()));
}

CommunicationClient::~CommunicationClient()
{
  if (m_state != IDLE) {
    qDebug() << "Still running";
  }
  delete m_socket;
}

void CommunicationClient::execute()
{
  moveToThread(&m_clientThread);
  m_clientThread.moveToThread(&m_clientThread);
  m_clientThread.start();
}

void CommunicationClient::connectToServer()
{
  m_socket = new QTcpSocket();
  connect(m_socket, SIGNAL(connected()), this, SLOT(signalConnected()));
  connect(m_socket, SIGNAL(disconnected()), this, SLOT(connectionClosedByServer()));
  connect(m_socket, SIGNAL(readyRead()), this, SLOT(receiveData()));
  if(m_socket->state() != QTcpSocket::ConnectedState)
    m_socket->connectToHost(QHostAddress::LocalHost, 5999);
}

void CommunicationClient::signalConnected()
{
  qDebug() << "true";
  emit connected(true);
  m_state = CONNECTED;
}

void CommunicationClient::receiveData()
{
  QDataStream in(m_socket);
  in.setVersion(QDataStream::Qt_4_8);
  forever
  {
    if(m_nextBlockSize == 0)
    {
      if(m_socket->bytesAvailable() < sizeof(quint16))
        break;
      in >> m_nextBlockSize;
    }
    if(m_socket->bytesAvailable() < m_nextBlockSize)
      break;

    quint8 opCode;
    QString str1;
    QString str2;
    quint16 port;

    in >> opCode >> str1 >> str2 >> port;
    processResponse(opCode, str1, str2, port);
    m_nextBlockSize = 0;
  }
}


void CommunicationClient::sendMessage()
{
  //Get message from queue;
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_4_8);
  //out << quint16(0) << name << pass << operation << (unsigned)messageID++ << uint1
  out.device()->seek(0);                          // 'scroll back' to that 0 at the beggining
  out << quint16(block.size() - sizeof(quint16)); // override it with actual data size
  m_socket->write(block);
}

void CommunicationClient::processResponse(quint8 opCode, QString str1, QString str2, quint16 port)
{
  //put message in queue
}

void CommunicationClient::connectionClosedByServer()
{
  m_socket->close();
  emit disconnected();
}

void CommunicationClient::closeConnection()
{
  connectionClosedByServer();
}
