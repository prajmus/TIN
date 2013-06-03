#include "messagequeue.h"
#include "networkqueue.h"
#include "opcodes.h"
#include "accountserver.h"
#include "filetransferserver.h"

#include <QMutexLocker>
#include <QEventLoop>
#include <QFile>

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
//  if (msg->str1[0] == '/') {
//    msg->str1.remove(0,1);
//  }
//  QString path = Client::getInstance().getPath() + msg->str1;

  if (msg->opCode == REGISTER) {
    if (AccountServer::getInstance().registerUser(msg->str1, msg->str2)) {
      msg = QSharedPointer<Message>(new Message(msg->sender, REGISTER_SUCCESSFUL, "", "", false));
      NetworkQueue::getInstance().addMessage(msg);
    }
    else {
      msg = QSharedPointer<Message>(new Message(msg->sender, REGISTER_FAILED, "", "", false));
      NetworkQueue::getInstance().addMessage(msg);
    }
  }
  else if (msg->opCode == LOGIN) {
    if (AccountServer::getInstance().verify(msg->str1, msg->str2)) {
      msg = QSharedPointer<Message>(new Message(msg->sender, LOGIN_SUCCESSFUL, "", "", false));
      NetworkQueue::getInstance().addMessage(msg);
    }
    else {
      msg = QSharedPointer<Message>(new Message(msg->sender, LOGIN_FAILED, "", "", false));
      NetworkQueue::getInstance().addMessage(msg);
    }
  }
  else if (msg->opCode == NEW_FILE) {
    QFile *file = new QFile(msg->str1);
    FileTransferServer transfer(file, false);

    transfer.execute();
    QEventLoop loop;

    loop.connect(&transfer, SIGNAL(listening()), SLOT(quit()));
    loop.connect(&transfer, SIGNAL(transferCompleted()), SLOT(quit()));

    loop.exec();

    quint16 port = transfer.getPort();
    msg = QSharedPointer<Message>(new Message(msg->sender, PUSH_FILE, msg->str1, "", false, port));

    loop.exec();



  }
}
