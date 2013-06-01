#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include "message.h"

#include <queue>
#include <QSharedPointer>
#include <QMutex>

class MessageQueue
{
    volatile bool running;
    std::queue< QSharedPointer<Message> > queue;
    QMutex mutex;
    void mainLoop();
    virtual void processOperation(QSharedPointer<Message> msg);
protected:
    MessageQueue();
public:
    ~MessageQueue();
    void addMessage(QSharedPointer<Message> msg);
    void stop();
};

//class MessageQueueIn : public MessageQueue {
//    void processOperation(QSharedPointer<Message> msg);
//public:
//    static MessageQueueIn &getInstance();
//};

//class MessageQueueOut : public MessageQueue {
//    void processOperation(QSharedPointer<Message> msg);
//public:
//    static MessageQueueOut &getInstance();
//};

#endif // MESSAGEQUEUE_H
