#ifndef COMMUNICATIONSERVER_H
#define COMMUNICATIONSERVER_H

#include <QObject>
#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QList>

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

    CommunicationServer(QObject *parent = 0);
    ~CommunicationServer();

    void execute();
    void stop();
  private slots:
    void addNewClient();
    void startListening();
    void disconnectSlot();
    void threadFinished();
  protected:
    QTcpServer *m_server;
    QList<QTcpSocket *> *m_clientList;
    QThread m_serverThread;
    QThread *m_parentThread;
    State m_state;
};

#endif // COMMUNICATIONSERVER_H
