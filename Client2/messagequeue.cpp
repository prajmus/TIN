#include "messagequeue.h"
#include "networkqueue.h"
#include "opcodes.h"
#include "client.h"
#include "fileserver.h"

#include <QMutexLocker>
#include <QDir>

MessageQueue::MessageQueue(QObject *parent)
{
  connect(this, SIGNAL(messageReady()), this, SLOT(processOperation()));
  connect(&FileServer::getInstance(), SIGNAL(fileDeleted(QString)), this, SLOT(fileDeleted(QString)));
  connect(&FileServer::getInstance(), SIGNAL(fileModified(QString)), this, SLOT(fileModified(QString)));
  connect(&FileServer::getInstance(), SIGNAL(newFile(QString)), this, SLOT(newFile(QString)));
}

MessageQueue::~MessageQueue()
{
  while (queue.size()) {
    queue.pop();
  }
}

void MessageQueue::addMessage(QSharedPointer<Message> msg)
{
    mutex.lock();
    queue.push(msg);
    mutex.unlock();
    emit messageReady();
}


MessageQueue& MessageQueue::getInstance()
{
  static MessageQueue instance;
  return instance;
}

void MessageQueue::processOperation()
{
  QSharedPointer<Message> msg = queue.front();
  queue.pop();
  if (msg->str1[0] == '/') {
    msg->str1.remove(0,1);
  }
  QString path = Client::getInstance().getPath() + msg->str1;

  if (msg->opCode == NEW_FILE) {
      //utwórz nowy plik
  }
  else if (msg->opCode ==  PUSH_FILE) {
      QFileInfo info(path);
      QString str = info.path();
      QDir dir(".");
      qDebug() << "Sending file " << dir.absolutePath() << ' '<< path;
      dir.mkpath(str);
      QFile *file = new QFile(path);
      FileTransferClient *transfer = new FileTransferClient(msg->port, file, true);
      transfer->execute();
  }
  else if (msg->opCode == PULL_FILE || msg->opCode == MODIFY_FILE) {
      QFileInfo info(path);
      QString str = info.path();
      QDir dir(".");
      qDebug() << dir.absolutePath() << ' ' << path;
      dir.mkpath(str);
      QFile *file = new QFile(path);
      FileTransferClient *transfer = new FileTransferClient(msg->port, file, false);
      transfer->execute();
  }
  else if (msg->opCode == DELETE_FILE) {
    FileServer::getInstance().removeFileFromDisk(msg->str1);
  }
}

void MessageQueue::fileDeleted(QString path)
{
  QSharedPointer<Message> msg = QSharedPointer<Message>(new Message(FILE_DELETED, path, "", true));
  NetworkQueue::getInstance().addMessage(msg);
}

void MessageQueue::fileModified(QString path)
{
  QSharedPointer<Message> msg = QSharedPointer<Message>(new Message(FILE_CHANGED, path, "", true));
  NetworkQueue::getInstance().addMessage(msg);
}

void MessageQueue::newFile(QString path)
{
  QSharedPointer<Message> msg = QSharedPointer<Message>(new Message(NEW_FILE, path, "", true));
  NetworkQueue::getInstance().addMessage(msg);
}

