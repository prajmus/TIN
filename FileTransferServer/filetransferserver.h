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
    FileTransferServer(int numberOfConnections, quint64 fileSize);
    ~FileTransferServer();

    bool startFileServer(const &QHostAddress = QHostAddress::Any, quint16 port = 0);

    QHostAddress getAddress();
    quint16 getPort();
    inline bool isRunning() { return (!state == OFFLINE); }

  signals:
    void serverStarted();

  private slots:
    void startServer();
    void addNewConnection();
    void readDataSlot();
    void socketDisconnected();
    void socketError();
    void threadFinished();
    void sendInitialization();
  private:

    //! The server socket
    QTcpServer *server;

    //! Pointer to the thread that created the object
    QThread *creatorThread;

    //! Thread used to handle file transfer
    QThread *additionalThread;

    //! List of connected clients
    QList<QTcpSocket* > clientList;

    //! Number of connected clients
    int numberOfConnections;

    //! Address of server
    QHostAddress address;

    //! Port number
    quint16 port;

    //! Size of file to be transfered
    quint64 fileSize;

    //! Current size of transfered file
    quint64 currentSize;

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

    State state;

};

#endif // FILETRANSFERSERVER_H
