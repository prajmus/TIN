#ifndef FILETRANSFERSERVER_H
#define FILETRANSFERSERVER_H

#include <QObject>
#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QList>

class FileTransferServer : public QObject
{
  Q_OBJECT
  public:
    FileTransferServer();
    ~FileTransferServer();

    bool startFileServer(const QHostAddress& address = QHostAddress::Any, quint16 port = 0);

    QHostAddress getAddress();
    quint16 getPort();
    inline bool isRunning() { return (!state_ == OFFLINE); }

  signals:
    void serverStarted();

  private slots:
    void startServer();
    void addNewConnection();
    void readDataSlot();
    void socketDisconnected();
    void socketError(QAbstractSocket::SocketError error);
    void threadFinished();
    void sendInitialization();
    void stopAll();
  private:

    //! The server socket
    QTcpServer *server_;

    //! Pointer to the thread that created the object
    QThread *creatorThread_;

    //! Thread used to handle file transfer
    QThread additionalThread_;

    //! List of connected clients
    QTcpSocket* clientList_;

    //! Number of connected clients
    int numberOfConnections_;

    //! Address of server
    QHostAddress address_;

    //! Port number
    quint16 port_;

    //! Size of file to be transfered
    quint64 fileSize_;

    //! Current size of transfered file
    quint64 currentSize_;

    //! State of server
    enum State {
      //! Nothing running
      OFFLINE,
      //! Running waiting for clients
      WAITING_FOR_CLIENTS,
      //! Clients connected, ready
      ALL_CONNECTED,
      //! File transfer started
      TRANSFER_IN_PROGRESS,
      //! Transfer completed
      TRANSFER_COMPLETED,
      //! There was an error
      ERROR,
      //! Forcing synchronous close
      CLOSING
    };

    State state_;

};

#endif // FILETRANSFERSERVER_H
