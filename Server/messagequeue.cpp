#include "messagequeue.h"

#include <QMutexLocker>

MessageQueue::MessageQueue()
{
    running = true;
}

MessageQueue::~MessageQueue()
{
    stop();
}

void MessageQueue::addMessage(QSharedPointer<Message> msg)
{
    QMutexLocker locker(&mutex);
    queue.push(msg);
    emit mes
}
