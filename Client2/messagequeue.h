#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include "message.h"
#include "client.h"
#include "filetransferclient.h"

#include <queue>
#include <QSharedPointer>
#include <QMutex>

class MessageQueue : public QObject
{
    Q_OBJECT
    std::queue< QSharedPointer<Message> > queue;
    QMutex mutex;
  private slots:
    void processOperation();
  protected:
    MessageQueue(QObject *parent = 0);
  public:
    ~MessageQueue();
    static MessageQueue& getInstance();
    void addMessage(QSharedPointer<Message> msg);
  signals:
    void messageReady();
};


#endif // MESSAGEQUEUE_H
