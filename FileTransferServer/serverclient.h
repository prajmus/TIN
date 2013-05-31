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
      TRANSFERING,
      ALL_READ,
      ALL_WRITTEN,
      FINISHED
    };

    enum Error {

    };

    ServerClient(QTcpSocket *socket, QObject *parent = 0);
    State state() const;
    QString stateString() const;
    Error error() const;
    QString errorString() const;

    void testFile();
  signals:
    void stateChanged(ServerClient::State state);
    void error(ServerClient::Error error);

    void downloadComplete();
    void dataSent(int uploadedBytes);
    void dataReceived(int receivedBytes);

    void stopped();
  private slots:
    void writeWelcome();
    void writeBytes(qint64);
  private:
    QFile *file;
    QTcpSocket *socket;
    QByteArray buffer;
    State m_state;
    Error m_error;
    qint64 writtenNow;
    qint64 writtenSoFar;
};

#endif // SERVERCLIENT_H
