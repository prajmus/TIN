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



const quint16 MIN_PORT = 6000;
const quint16 MAX_PORT = 6100;

class FileTransferServer : public QObject
{
  Q_OBJECT
  private:
    enum State {
      IDLE,
      LISTENING,
      CONNECTED
    };

  public:
    FileTransferServer(QFile *file, bool isSender, QObject *parent = 0);
    ~FileTransferServer();

    void execute();
    void stop();
  private slots:
    void addNewClient();
    void startListening();
    void disconnectSlot();
    void threadFinished();
  protected:
    QTcpServer *m_server;
    ServerClient *m_socket;
    QFile *m_file;
    ServerClient* m_clients;
    QThread m_serverThread;
    QThread *m_parentThread;
    State m_state;
    bool m_sender;
};

#endif // FILETRANSFERSERVER_H
