#include "client.h"

#include <QHostAddress>
#include <QDataStream>
#include <QDebug>


Client::Client(quint16 port, QFile *file, bool isSender, QObject *parent) : m_port(port), m_file(file),
  m_sender(isSender), m_state(IDLE)
{
  m_parentThread = QThread::currentThread();
  connect(&m_clientThread, SIGNAL(started()), this, SLOT(connectToServer()));

//  file = new QFile("obraz.png");
//  file->open(QIODevice::WriteOnly);
}

Client::~Client()
{
  if(m_state != IDLE) {
    qDebug() << "Still runnin'";

  }
  delete m_socket;
}

void Client::execute()
{
  moveToThread(&m_clientThread);
  m_clientThread.moveToThread(&m_clientThread);
  m_clientThread.start();
}

void Client::connectToServer()
{
  m_socket = new QTcpSocket();
  qDebug() << "connecting";
  connect(m_socket, SIGNAL(connected()), this, SLOT(signalConnected()));
  connect(m_socket, SIGNAL(disconnected()), this, SLOT(connectionClosedByServer()));
  connect(m_socket, SIGNAL(readyRead()), this, SLOT(receiveData()));
  connect(m_socket, SIGNAL(bytesWritten(qint64)), this, SLOT(writeBytes(qint64)));
  if(m_socket->state() != QTcpSocket::ConnectedState)
    m_socket->connectToHost(QHostAddress::LocalHost, m_port);



}

void Client::signalConnected()
{
  qDebug() << "true";
  emit connected(true);
  m_state = CONNECTED;
  if(m_sender)
    sendFile();
}

void Client::error()
{
  m_socket->close();
}

void Client::receiveData()
{
  if(m_state < INITIALIZED) {
    getInit();
  }
    if(!m_file->isOpen())
      m_file->open(QIODevice::WriteOnly | QIODevice::Truncate);

  if(m_state != TRANSFER_FINISHED || m_currentlyReceived != m_fileSize) {
    if (m_state == INITIALIZED) {
      m_state = TRANSFERING;

      QDataStream in(m_socket);
      in >> m_fileSize;
    }
    qDebug() << m_fileSize;
    QByteArray buffer = m_socket->read(m_fileSize);
    m_currentlyReceived += buffer.size();

    m_file->write(buffer);

    if (m_currentlyReceived == m_fileSize) {
      m_file->close();
      delete m_file;
      m_state = TRANSFER_FINISHED;

      m_socket->disconnectFromHost();

    } else if (m_currentlyReceived > m_fileSize) {
      qDebug() << "Received too large file";
      m_state = ERROR;

      m_socket->disconnectFromHost();
    }
  }
  else
  {
    QByteArray buffer2 = m_socket->readAll();
    qDebug() << buffer2;
  }
}

void Client::getInit()
{
  m_socket->read(1);
  m_state = INITIALIZED;
  if(m_sender == true)
    sendFile();
}

void Client::sendFile()
{
  m_file->open(QIODevice::ReadOnly);
  m_state = TRANSFERING;

  QByteArray fileSize;
  QDataStream in(&fileSize, QIODevice::WriteOnly);
  in << m_file->size();


  m_buffer = m_file->read(8096);
  //added if block to file transfer defect fixed
  if (m_file->atEnd()) {
    m_state = ALL_READ;
    m_file->close();
  }
  qint64 written = m_socket->write(m_buffer);


  m_buffer = m_buffer.right(m_buffer.size() - written);

  //added if block to file transfer defect fixed
  if ((m_state == ALL_READ) && (m_buffer.size() == 0)) {
    m_state = ALL_WRITTEN;
  }

}

void Client::writeBytes(qint64 bytes)
{
  if (m_state == ALL_WRITTEN) {
    if (m_socket->bytesToWrite() != 0) {
      return;
    }

    m_state = TRANSFER_FINISHED;
    disconnect(m_socket, SIGNAL(bytesWritten(qint64)), this,
               SLOT(writeBytes(qint64)));

    m_socket->disconnectFromHost();
  } else {
    if ((!(m_state == ALL_READ)) && (m_buffer.size() < 8096)) {
      m_buffer.append(m_file->read(8096 - m_buffer.size()));

      if (m_file->atEnd()) {
        m_state = ALL_READ;
        m_file->close();
      }
    }

    qint64 written = m_socket->write(m_buffer);

    m_buffer = m_buffer.right(m_buffer.size() - written);

    if ((m_state == ALL_READ) && (m_buffer.size() == 0)) {
      m_state = ALL_WRITTEN;
    }
  }
}
void Client::connectionClosedByServer()
{
  m_socket->close();
  emit disconnected();
}

void Client::closeConnection()
{

}

