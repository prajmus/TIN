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
    if (!running)
        return;
    QMutexLocker locker(&mutex);
    queue.push(msg);
}


void MessageQueue::stop()
{
    if (!running)
        return;
    running = false;
}
