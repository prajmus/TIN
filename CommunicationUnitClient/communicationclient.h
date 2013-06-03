#ifndef COMMUNICATIONCLIENT_H
#define COMMUNICATIONCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>

class CommunicationClient : public QObject
{
    Q_OBJECT
  public:
    enum State {
      IDLE,
      CONNECTED,
      INITIALIZED,
      TRANSFERING,
      ALL_READ,
      ALL_WRITTEN,
      TRANSFER_FINISHED,
      ERROR
    };

    CommunicationClient(QObject *parent = 0);
    ~CommunicationClient();
    
    void execute();
    void closeConnection();
  public slots:
    void connectToServer();
    void signalConnected();
    void error();
    void receiveData();
    void connectionClosedByServer();
    void sendMessage();
  signals:
    void connected(bool);
    void disconnected();
  private:
    State m_state;
    QTcpSocket *m_socket;
    QByteArray m_buffer;
    QThread *m_parentThread;
    QThread m_clientThread;
    qint64 m_nextBlockSize;
    bool m_sender;
    void processResponse(quint8, QString, QString, quint16);
};

#endif // COMMUNICATIONCLIENT_H
