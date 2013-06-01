#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QFile>
#include <QThread>

class Client : public QObject
{
    Q_OBJECT
  public:
    enum State{
      IDLE,
      CONNECTED,
      INITIALIZED,
      TRANSFERING,
      ALL_READ,
      ALL_WRITTEN,
      TRANSFER_FINISHED,
      ERROR
    };


    Client(quint16 port, QFile *file, bool isSender, QObject *parent = 0);
    ~Client();
    void sendFile();
    void execute();
  public slots:
    void connectToServer();
    void signalConnected();
    void error();
    void receiveData();
    void connectionClosedByServer();
    void writeBytes(qint64);
  signals:
    void connected(bool);
    void disconnected();
  private:
    qint64 m_currentlyReceived;
    qint64 m_fileSize;
    quint16 m_port;
    State m_state;
    QFile *m_file;
    QTcpSocket *m_socket;
    QByteArray m_buffer;
    QThread *m_parentThread;
    QThread m_clientThread;
    bool m_sender;
    void getInit();
    void closeConnection();
};

#endif // CLIENT_H
