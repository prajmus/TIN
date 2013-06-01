#include "serverclient.h"
#include <QDataStream>
#include <QDebug>
#include <QFile>

ServerClient::ServerClient(QTcpSocket *socket, QFile *file, bool isSender, QObject *parent) :
  m_socket(socket), m_file(file), m_sender(isSender)
{
  connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(writeBytes(qint64)));
  connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
  connect(socket, SIGNAL(readyRead()), this, SLOT(receiveData()));
}


void ServerClient::sendInit()
{
  m_socket->write("s");
  qDebug() << "po wysylaniu";
}

void ServerClient::sendFile()
{
  m_file->open(QIODevice::ReadOnly);
  m_state = TRANSFERING;
  writtenSoFar = 0;                               //usun writtensofar
  QByteArray fileSize;
  QDataStream in(&fileSize, QIODevice::WriteOnly);
  in << m_file->size();

  writtenNow = m_socket->write(fileSize); // usun writtenNow

  qDebug() << writtenNow;

  m_buffer = m_file->read(8096);
  //added if block to file transfer defect fixed
  if (m_file->atEnd()) {
    m_state = ALL_READ;
    m_file->close();
  }
  qint64 written = m_socket->write(m_buffer);
  /*------------------------------------------------------------*/
  writtenSoFar += written;
  qDebug() << writtenSoFar << " " << written;
  /*------------------------------------------------------------*/

  m_buffer = m_buffer.right(m_buffer.size() - written);

  //added if block to file transfer defect fixed
  if ((m_state == ALL_READ) && (m_buffer.size() == 0)) {
    m_state = ALL_WRITTEN;
    m_socket->flush();
  }

}

void ServerClient::writeBytes(qint64 bytes)
{
  if (m_state == TRANSFERING) {
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
      /*------------------------------------------------------------*/
      writtenSoFar += written;
      qDebug() << writtenSoFar << " " << written;
      /*------------------------------------------------------------*/
      m_buffer = m_buffer.right(m_buffer.size() - written);

      if ((m_state == ALL_READ) && (m_buffer.size() == 0)) {
        m_state = ALL_WRITTEN;
      }
    }
  }
}

void ServerClient::receiveData()
{
  if(m_state != TRANSFER_FINISHED) {
    if (m_state == CONNECTED) {
      m_state = TRANSFERING;

      QDataStream in(m_socket);
      in >> m_fileSize;
    }

    QByteArray buffer = m_socket->read(m_fileSize);
    m_currentlyReceived += buffer.size();

    m_file->write(buffer);

    if (m_currentlyReceived == m_fileSize) {
      m_file->close();
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


void ServerClient::onDisconnected()
{
  emit disconnected();
}

void ServerClient::onConnected()
{
  emit connected();
}
