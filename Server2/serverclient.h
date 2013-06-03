#ifndef SERVERCLIENT_H
#define SERVERCLIENT_H

#include <QObject>
#include <QTcpSocket>

class QFile;

class ServerClient : public QObject
{
    Q_OBJECT
  public:
    enum State {
      CONNECTED,
      TRANSFERING,
      ALL_READ,
      ALL_WRITTEN,
      TRANSFER_FINISHED,
      ERROR
    };

    enum Error {

    };

    ServerClient(QTcpSocket *socket, QFile *file, bool isSender, QObject *parent = 0);
    State state() const;
    QString stateString() const;
    Error error() const;
    QString errorString() const;
    void sendInit();
    void sendFile();
  signals:
    void stateChanged(ServerClient::State state);
    void error(ServerClient::Error error);
    void disconnected();
    void downloadComplete();
    void dataSent(int uploadedBytes);
    void dataReceived(int receivedBytes);
    void connected();
    void stopped();
  private slots:
    void onDisconnected();
    void onConnected();
    void writeBytes(qint64);
    void receiveData();
  private:
    qint64 m_currentlyReceived;
    qint64 m_fileSize;
    QTcpSocket *m_socket;
    QFile *m_file;
    QByteArray m_buffer;
    State m_state;
    Error m_error;
    qint64 writtenNow;
    qint64 writtenSoFar;
    bool m_sender;
};

#endif // SERVERCLIENT_H
