#ifndef NETWORKQUEUE_H
#define NETWORKQUEUE_H

#include "message.h"

#include <queue>

#include <QObject>
#include <QSharedPointer>
#include <QMutex>

class NetworkQueue : public QObject
{
    Q_OBJECT
private:
    std::queue< QSharedPointer<Message> > queue;
    QMutex mutex;
protected:
    NetworkQueue(QObject* parent = 0);
public:
    ~NetworkQueue();
    static NetworkQueue &getInstance();
    void addMessage(QSharedPointer<Message> msg);
    QSharedPointer<Message> pop();
signals:
    void messageReady();
};

#endif // NETWORKQUEUE_H
