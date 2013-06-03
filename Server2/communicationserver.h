#ifndef COMMUNICATIONSERVER_H
#define COMMUNICATIONSERVER_H

#include <QObject>
#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QList>

#include "networkqueue.h"

const quint16 PORT = 5999;

class CommunicationServer : public QObject
{
    Q_OBJECT
  public:
    enum State {
      IDLE,
      LISTENING,
      CONNECTED
    };

    ~CommunicationServer();
    static CommunicationServer& getInstance();
    QList<QTcpSocket *> *m_clientList;
    void execute();
    void stop();
  private slots:
    void addNewClient();
    void startListening();
    void disconnectSlot();
    void threadFinished();
    void sendMessage();
    void readData();
    void listFiles(QTcpSocket *);
    void processResponse(QTcpSocket*, quint8, QString, QString, quint16);
    void spreadFile(QString , QTcpSocket *);
  protected:
    CommunicationServer(QObject *parent = 0);
    QTcpServer *m_server;
    QThread m_serverThread;
    QThread *m_parentThread;
    State m_state;
    qint16 m_nextBlockSize;
};

#endif // COMMUNICATIONSERVER_H
