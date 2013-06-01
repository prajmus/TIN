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
    emit messageReady(msg);
}
