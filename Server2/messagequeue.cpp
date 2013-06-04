#include "messagequeue.h"
#include "networkqueue.h"
#include "opcodes.h"
#include "accountserver.h"
#include "filetransferserver.h"
#include "fileserver.h"

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
//    QMutexLocker locker(&mutex);
    mutex.lock();
    queue.push(msg);
    mutex.unlock();
    emit messageReady();
}

void MessageQueue::transferFinished(QString file, bool sender, QTcpSocket* fromWho)
{
  FileServer::getInstance().addFileToList(file, QDateTime::currentDateTime());
  if(sender)
    return;
  else
    emit spreadFile(file, fromWho);
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
  else if (msg->opCode == NEW_FILE || msg->opCode == MODIFY_FILE) {
    qDebug() << msg->str1;
    if(msg->opCode == MODIFY_FILE) {
      FileServer::getInstance().removeFile(msg->str1);
    }
    FileTransferServer *transfer = new FileTransferServer(PATH+msg->str1, false, msg->sender);

    QEventLoop loop;
    loop.connect(transfer, SIGNAL(listening()), SLOT(quit()));
    transfer->execute();


    loop.exec();

    quint16 port = transfer->getPort();
    msg = QSharedPointer<Message>(new Message(msg->sender, PUSH_FILE, msg->str1, "", false, true, port));
    NetworkQueue::getInstance().addMessage(msg);
  }
  else if (msg->opCode == DELETE_FILE) {
    FileServer::getInstance().removeFile(msg->str1);
    msg = QSharedPointer<Message>(new Message(msg->sender, DELETE_FILE, msg->str1, "", false, false));
    NetworkQueue::getInstance().addMessage(msg);
  }
  else if (msg->opCode == LIST_FILES) {
    emit listFiles(msg->sender);
  }
  else if(msg->opCode == REQ_FILE) {
    FileTransferServer *transfer = new FileTransferServer(PATH+msg->str1, true, msg->sender);

    transfer->execute();
    QEventLoop loop;

    loop.connect(transfer, SIGNAL(listening()), SLOT(quit()));

    loop.exec();

    quint16 port = transfer->getPort();
    msg = QSharedPointer<Message>(new Message(msg->sender, PULL_FILE, msg->str1, "", false,true,port));
    NetworkQueue::getInstance().addMessage(msg);
  }
}
