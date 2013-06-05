#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include "message.h"
#include "filetransferserver.h"

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
    QString trimm(QString path);
  public:
    ~MessageQueue();
    static MessageQueue& getInstance();
    void addMessage(QSharedPointer<Message> msg);
    void transferFinished(QString, bool sender, QTcpSocket *fromWho);
  signals:
    void messageReady();
    void spreadFile(QString, QTcpSocket*);
    void listFiles(QTcpSocket*);
};


#endif // MESSAGEQUEUE_H
