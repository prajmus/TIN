#include "communicationclient.h"
#include "opcodes.h"
#include "messagequeue.h"
#include "networkqueue.h"
#include "message.h"
#include "client.h"

#include <QHostAddress>
#include <QDataStream>
#include <QDateTime>

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

CommunicationClient &CommunicationClient::getInstance()
{
  static CommunicationClient instance;
  return instance;
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
  connect(&m_clientThread, SIGNAL(finished()), this, SLOT(threadFinished()));
  connect(m_socket, SIGNAL(readyRead()), this, SLOT(receiveData()));
  connect(&NetworkQueue::getInstance(), SIGNAL(messageReady()), this, SLOT(sendMessage()));
  if(m_socket->state() != QTcpSocket::ConnectedState)
    m_socket->connectToHost(SERVER_ADDRESS, 5999);
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

    in >> opCode;
    if(opCode != FILES_LISTING)
      in >> str1 >> str2 >> port;
    else {
      quint32 numberOfFiles;
      QList<std::pair<QString, QDateTime> > *files = new QList<std::pair<QString, QDateTime> >();
      QDateTime date;
      in >> numberOfFiles;
      while (numberOfFiles--) {
        in >> str1 >> date;
        files->append(std::make_pair(str1, date));
      }
      Client::getInstance().setRemoteList(files);
      Client::getInstance().compareLocalCopies();
      m_nextBlockSize = 0;
      return;
    }
    processResponse(opCode, str1, str2, port);
    m_nextBlockSize = 0;
  }
}


void CommunicationClient::sendMessage()
{
  QSharedPointer<Message> msg = NetworkQueue::getInstance().pop();
  QByteArray block;
  qDebug() << "Sending " << msg->opCode;
  QDataStream out(&block, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_4_8);
  out << quint16(0) << msg->opCode << msg->str1 << msg->str2 << msg->port;
  out.device()->seek(0);                          // 'scroll back' to that 0 at the beggining
  out << quint16(block.size() - sizeof(quint16)); // override it with actual data size
  m_socket->write(block);
}

void CommunicationClient::processResponse(quint8 opCode, QString str1, QString str2, quint16 port)
{
  qDebug() << "Process Response" << opCode << ' ' << str1 << ' ' << port;
  QSharedPointer<Message> msg = QSharedPointer<Message>(new Message(opCode, str1, str2, false, port));
  switch (opCode) {
    case REGISTER_SUCCESSFUL:
      Client::getInstance().setLoggedIn(true);
      emit registerSuccessful();
      break;
    case REGISTER_FAILED:
      Client::getInstance().setLoggedIn(false);
      emit registerFailed();
      break;
    case LOGIN_SUCCESSFUL:
      Client::getInstance().setLoggedIn(true);
      emit loginSuccessful();
      break;
    case LOGIN_FAILED:
      Client::getInstance().setLoggedIn(false);
      emit loginFailed();
      break;
    default:
      MessageQueue::getInstance().addMessage(msg);

  }
}

void CommunicationClient::threadFinished()
{
    m_state = IDLE;
    this->deleteLater();
}

void CommunicationClient::connectionClosedByServer()
{
    moveToThread(m_parentThread);
    m_clientThread.moveToThread(m_parentThread);
    m_socket->close();
    m_clientThread.quit();
    emit disconnected();
}

void CommunicationClient::closeConnection()
{
  connectionClosedByServer();
}
