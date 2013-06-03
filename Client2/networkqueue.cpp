#include "networkqueue.h"
#include "QMutexLocker"

NetworkQueue::NetworkQueue(QObject *parent)
{
}

NetworkQueue::~NetworkQueue()
{
    while (queue.size()) {
        queue.front()->return_msg(false);
        queue.pop();
    }
}

NetworkQueue &NetworkQueue::getInstance()
{
    static NetworkQueue instance;
    return instance;
}

void NetworkQueue::addMessage(QSharedPointer<Message> msg)
{
    QMutexLocker locker(&mutex);
    queue.push(msg);
    emit messageReady();
}

QSharedPointer<Message> NetworkQueue::pop()
{
  QMutexLocker locker(&mutex);
  QSharedPointer<Message> msg = queue.front();
  queue.pop();
  return msg;
}
