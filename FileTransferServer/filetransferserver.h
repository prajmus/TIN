#ifndef FILETRANSFERSERVER_H
#define FILETRANSFERSERVER_H

#include <QObject>
#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QTimer>
#include <QList>

#include "serverclient.h"

class QFile;

const quint16 PORT_NUMBER = 6018;

class FileTransferServer : public QObject
{
  Q_OBJECT
  public:
    static FileTransferServer& getInstance();
    void stop();
  public slots:
    void execute();
  private slots:
    void timerAction();
    void addNewClient();
  protected:
    inline FileTransferServer(QObject *parent = 0);
    QTcpServer server;
    QList<ServerClient* > clients;
    QTimer *timer;
    QThread serverThread;

//    inline bool isRunning() { return (!state_ == OFFLINE); }

//  signals:
//    void serverStarted();
//    void finished();
//  private slots:
//    bool startFileServer();
//    void addNewConnection();
//    void socketDisconnected();
//    void bytesWrittenSlot(qint64 bytes);
//  private:
//    void sendFile();
//    QFile *file;
//    QByteArray buffer;
//    qint64 bufferSize;
//    //! The server socket
//    QTcpServer *server_;

//    //! List of connected clients
//    QTcpSocket* clientList_;

//    //! Number of connected clients
//    int numberOfConnections_;

//    //! Address of server
//    QHostAddress address_;

//    //! Port number
//    quint16 port_;

//    //! Size of file to be transfered
//    quint64 fileSize_;

//    //! Current size of transfered file
//    quint64 currentSize_;

//    //! State of server
//    enum State {
//      //! Nothing running
//      OFFLINE,
//      //! Running waiting for clients
//      WAITING_FOR_CLIENTS,
//      //! Clients connected, ready
//      ALL_CONNECTED,
//      //! File transfer started
//      TRANSFER_IN_PROGRESS,
//      //! Transfer completed
//      TRANSFER_COMPLETED,
//      //! There was an error
//      ERROR,

//      ALL_DATA_READ,

//      ALL_DATA_WRITTEN,
//      //! Forcing synchronous close
//      CLOSING
//    };

//    State state_;

};

#endif // FILETRANSFERSERVER_H
