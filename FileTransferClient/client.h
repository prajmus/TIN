#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QFile>

class Client : public QObject
{
    Q_OBJECT
  private:
    Client(QObject *parent = 0);

    enum State{
      CONNECTED,
      TRANSFERING,
      TRANSFER_FINISHED,
      ERROR
    };
    State m_state;
    QString name;
    QString pass;
    QString host;
    QTcpSocket socket;

    quint16 blockSize;
    qint64 m_currentlyReceived;
    QFile *file;
    qint64 m_fileSize;
    void closeConnection();

    void sendRequest();
    void sendFile();
  public:
    static Client& getInstance();
    void connectToServer();
  public slots:
    void signalConnected();
    void error();
    void receiveData();
    void connectionClosedByServer();
  signals:
    void connected(bool);
    void disconnected();

};

#endif // CLIENT_H
