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
  public:
    FileTransferServer(QFile *file, QObject *parent = 0);
    ~FileTransferServer();
    void stop();
  public slots:
    void execute();
  private slots:
    void timerAction();
    void addNewClient();
    void startListening();
  protected:
    QTcpServer *server;
    ServerClient *socket;
    QFile *m_file;
    ServerClient* clients;
    QTimer *timer;
    QThread serverThread;
    QThread *parentThread;
};

#endif // FILETRANSFERSERVER_H
