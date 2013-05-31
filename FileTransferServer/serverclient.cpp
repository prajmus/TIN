#include "serverclient.h"
#include <QDataStream>
#include <QDebug>
#include <QFile>

ServerClient::ServerClient(QTcpSocket *socket, QObject *parent)
{
  this->socket = socket;

  connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(writeBytes(qint64)));
  //writeWelcome();
}


void ServerClient::writeWelcome()
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_5_0);
  QString t = "oleole";
  out << quint16(0) << t;
  out.device()->seek(0);                          // 'scroll back' to that 0 at the beggining
  out << quint16(block.size() - sizeof(quint16)); // override it with actual data size
  socket->write(block);
  qDebug() << socket->socketDescriptor();
  qDebug() << "po wysylaniu";
}

void ServerClient::testFile()
{
  file = new QFile("obraz.png");
  file->open(QIODevice::ReadOnly);
  m_state = TRANSFERING;
  writtenSoFar = 0;                               //usun writtensofar
  QByteArray fileSize;
  QDataStream in(&fileSize, QIODevice::WriteOnly);
  in << file->size();

  writtenNow = socket->write(fileSize); // usun writtenNow

  qDebug() << writtenNow;

  buffer = file->read(8096);
  //added if block to file transfer defect fixed
  if (file->atEnd()) {
    m_state = ALL_READ;
    file->close();
    delete file;
  }
  qint64 written = socket->write(buffer);
  /*------------------------------------------------------------*/
  writtenSoFar += written;
  qDebug() << writtenSoFar << " " << written;
  /*------------------------------------------------------------*/

  buffer = buffer.right(buffer.size() - written);

  //added if block to file transfer defect fixed
  if ((m_state == ALL_READ) && (buffer.size() == 0)) {
    m_state = ALL_WRITTEN;
    socket->flush();
  }

}

void ServerClient::writeBytes(qint64 bytes)
{
  if (m_state == ALL_WRITTEN) {
    if (socket->bytesToWrite() != 0) {
      return;
    }

    m_state = FINISHED;
    disconnect(socket, SIGNAL(bytesWritten(qint64)), this,
               SLOT(writeBytes(qint64)));

    socket->disconnectFromHost();
  } else {
    if ((!(m_state == ALL_READ)) && (buffer.size() < 8096)) {
      buffer.append(file->read(8096 - buffer.size()));

      if (file->atEnd()) {
        m_state = ALL_READ;
        file->close();
        delete file;
      }
    }

    qint64 written = socket->write(buffer);
    /*------------------------------------------------------------*/
    writtenSoFar += written;
    qDebug() << writtenSoFar << " " << written;
    /*------------------------------------------------------------*/
    buffer = buffer.right(buffer.size() - written);

    if ((m_state == ALL_READ) && (buffer.size() == 0)) {
      m_state = ALL_WRITTEN;
    }
  }
}
