#include <QSharedPointer>
#include <QDataStream>
#include <QEventLoop>

#include "communicationserver.h"
#include "messagequeue.h"
#include "fileserver.h"

#include "message.h"
#include "opcodes.h"

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

CommunicationServer &CommunicationServer::getInstance()
{
  static CommunicationServer instance;
  return instance;
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
  connect(&NetworkQueue::getInstance(), SIGNAL(messageReady()), this, SLOT(sendMessage()));
  connect(&MessageQueue::getInstance(), SIGNAL(spreadFile(QFile*,QTcpSocket*)), this, SLOT(spreadFile));
  connect(&MessageQueue::getInstance(), SIGNAL(listFiles(QTcpSocket*)), this, SLOT(listFiles(QTcpSocket*)));
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

void CommunicationServer::sendMessage()
{
  QSharedPointer<Message> msg = NetworkQueue::getInstance().pop();
  if(msg->toOne) {
    QTcpSocket *sender = msg->sender;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << msg->opCode << msg->str1 << msg->str2 << msg->port;
    out.device()->seek(0);                          // 'scroll back' to that 0 at the beggining
    out << quint16(block.size() - sizeof(quint16)); // override it with actual data size
    sender->write(block);
    qDebug() << msg->opCode << sender->socketDescriptor();
  }
  else {
    foreach(QTcpSocket *s , (*m_clientList)) {
      if(s == msg->sender) continue;
      QByteArray block;
      QDataStream out(&block, QIODevice::WriteOnly);
      out.setVersion(QDataStream::Qt_4_8);
      out << quint16(0) << msg->opCode << msg->str1 << msg->str2 << msg->port;
      out.device()->seek(0);                          // 'scroll back' to that 0 at the beggining
      out << quint16(block.size() - sizeof(quint16)); // override it with actual data size
      s->write(block);
    }
  }
}

void CommunicationServer::readData()
{
  QTcpSocket *receiver = qobject_cast<QTcpSocket*>(QObject::sender());
  QDataStream in(receiver);
  in.setVersion(QDataStream::Qt_4_8);
  forever
  {
    if(m_nextBlockSize == 0)
    {
      if(receiver->bytesAvailable() < sizeof(quint16))
        break;
      in >> m_nextBlockSize;
    }
    if(receiver->bytesAvailable() < m_nextBlockSize)
      break;

    quint8 opCode;
    QString str1;
    QString str2;
    quint16 port;

    in >> opCode >> str1 >> str2 >> port;
    qDebug() << opCode << str1 << receiver->socketDescriptor();

    processResponse(receiver, opCode, str1, str2, port);
    m_nextBlockSize = 0;
  }
}

void CommunicationServer::listFiles(QTcpSocket * socket)
{
  std::vector<std::pair<QString, QDateTime> > t = FileServer::getInstance().listFiles();
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_4_8);
  out << quint16(0) << FILES_LISTING;
  for(auto s : t) {
    out << s.first << s.second;
  }
  out.device()->seek(0);
  out << quint16(block.size() - sizeof(quint16));
  socket->write(block);
}

void CommunicationServer::processResponse(QTcpSocket * receiver, quint8 opCode, QString str1, QString str2, quint16 port)
{
  QSharedPointer<Message> msg = QSharedPointer<Message>(new Message(receiver, opCode, str1, str2, false, port));
  MessageQueue::getInstance().addMessage(msg);
}

void CommunicationServer::spreadFile(QString path, QTcpSocket * socket)
{
  foreach(QTcpSocket *s , (*m_clientList)) {
    if(s == socket) continue;

    FileTransferServer transfer(path, true, s);

    transfer.execute();
    QEventLoop loop;

    loop.connect(&transfer, SIGNAL(listening()), SLOT(quit()));

    loop.exec();

    quint16 port = transfer.getPort();
    QSharedPointer<Message> msg = QSharedPointer<Message>(new Message(s, PUSH_FILE, path, "", false, port));
    NetworkQueue::getInstance().addMessage(msg);
  }
}

void CommunicationServer::addNewClient()
{
  while(m_server->hasPendingConnections()) {
    QTcpSocket *client = m_server->nextPendingConnection();
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnectSlot()));
    connect(client, SIGNAL(readyRead()), this, SLOT(readData()));
    qDebug() << client->socketDescriptor();
    m_clientList->append(client);
    m_state = CONNECTED;
    connect(&m_serverThread, SIGNAL(finished()), this, SLOT(threadFinished()));
  }
}
