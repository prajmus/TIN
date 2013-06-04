#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QCoreApplication>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QDateTime>

#include "paths.h"

class Client
{
  private:
    QCoreApplication *app;
    QString path;           // monitored by client
    QString login;
    QString password;
    QList<std::pair<QString, QDateTime> > *remoteList;
<<<<<<< HEAD
    bool loggedIn = false;
=======
    bool loggedIn;

protected:
>>>>>>> ea35973d635b944fc3cce8b25a9df9718a6b9b02
    Client();

  public:
    void run();
    static Client& getInstance();

    QStringList* readConfigFile(QFile & file);
    bool loginAvailable(QString login);
    void createAccount();
    void registerUser(QString login, QString password);
    void connectToServer();
    void terminateClient();
    bool compareLocalCopies();
    void synchronizeFiles();

    void showStatus();
    void showMonitoredFiles();

    void logIn();
    void logToServer();
    void createConfigFile();
    void setLoggedIn(bool);
    void setRemoteList(QList<std::pair<QString, QDateTime> >*);
    QString getPath() const;

    void showManageUsage();
    void showFolderUsers();
    void addFolderUser(QStringList loginList);
    void removeFolderUser(QStringList loginList);

    void listCommands();
  signals:
    void finished();
public slots:
    void aboutToQuitApp();
    void quit();
    void fileDeleted(QString);
    void fileModified(QString);
    void newFile(QString);
};

#endif // CLIENT_H
