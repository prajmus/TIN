#include "messagequeue.h"
#include "opcodes.h"
#include "client.h"

#include <QMutexLocker>

MessageQueue::MessageQueue(QObject *parent)
{
  connect(this, SIGNAL(messageReady()), this, SLOT(processOperation()));
}

MessageQueue::~MessageQueue()
{
  while (queue.size()) {
    queue.pop();
  }
}

void MessageQueue::addMessage(QSharedPointer<Message> msg)
{
    QMutexLocker locker(&mutex);
    queue.push(msg);
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

  if(msg->opCode == NEW_FILE) {
      //utwórz nowy plik
  }
  else if (msg->opCode==  PUSH_FILE) {
      QFile *file = new QFile(path);
      FileTransferClient transfer(msg->port, file, true);
      transfer.execute();
  }
  else if (msg->opCode == PULL_FILE || msg->opCode == MODIFY_FILE) {
      QFile *file = new QFile(path);
      FileTransferClient transfer(msg->port, file, false);
      transfer.execute();
  }
  else if (msg->opCode == DELETE_FILE) {
      //deleteFile
  }
}
