#include "client.h"

#include <QHostAddress>
#include <QDataStream>
#include <QDebug>


Client::Client(QObject *parent)
{
  connect(&socket, SIGNAL(connected()), this, SLOT(signalConnected()));
  connect(&socket, SIGNAL(disconnected()), this, SLOT(connectionClosedByServer()));
  connect(&socket, SIGNAL(readyRead()), this, SLOT(receiveData()));
  file = new QFile("obraz.png");
  file->open(QIODevice::WriteOnly);
}

Client& Client::getInstance(){
  static Client instance;
  return instance;
}

void Client::connectToServer()
{
  qDebug() << "connecting";
  if(socket.state() != QTcpSocket::ConnectedState)
    socket.connectToHost(QHostAddress::LocalHost, 6018);

  blockSize = 0;
}

void Client::signalConnected()
{
  qDebug() << "true";
  emit connected(true);
}

void Client::error()
{
  socket.close();
}

void Client::receiveData()
{
  if(m_state != TRANSFER_FINISHED) {
    if (m_state == CONNECTED) {
      m_state = TRANSFERING;

      QDataStream in(&socket);
      in >> m_fileSize;
    }

    QByteArray buffer = socket.read(m_fileSize);
    m_currentlyReceived += buffer.size();

    file->write(buffer);

    if (m_currentlyReceived == m_fileSize) {
      file->close();
      delete file;
      m_state = TRANSFER_FINISHED;

      socket.disconnectFromHost();

    } else if (m_currentlyReceived > m_fileSize) {
      qDebug() << "Received too large file";
      m_state = ERROR;

      socket.disconnectFromHost();
    }
  }
  else
  {
    QByteArray buffer2 = socket.readAll();
    qDebug() << buffer2;
  }
}

void Client::sendRequest()
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_4_8);
  out << quint16(0) << 10 << "prajmus" << "haslo";
  out.device()->seek(0);                          // 'scroll back' to that 0 at the beggining
  out << quint16(block.size() - sizeof(quint16)); // override it with actual data size
  socket.write(block);
}

void Client::connectionClosedByServer()
{
  socket.close();
  emit disconnected();
}

void Client::closeConnection()
{

}

