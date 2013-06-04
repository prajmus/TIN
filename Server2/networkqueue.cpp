#include "networkqueue.h"
#include "QMutexLocker"

NetworkQueue::NetworkQueue(QObject *parent)
{
}

NetworkQueue::~NetworkQueue()
{
    while (queue.size()) {
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
    mutex.lock();
    queue.push(msg);
    mutex.unlock();
    emit messageReady();
}

QSharedPointer<Message> NetworkQueue::pop()
{
//  QMutexLocker locker(&mutex);
  mutex.lock();
  QSharedPointer<Message> msg = queue.front();
  mutex.unlock();
  queue.pop();
  return msg;
}
