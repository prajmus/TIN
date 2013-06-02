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
    bool loggedIn = false;

protected:
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
    QList< std::pair<QString,QDateTime> >* getRemoteList();
    void synchronizeFiles();

    void showStatus();
    void showMonitoredFiles();

    void logIn();
    void logToServer();
    void createConfigFile();

    void showManageUsage();
    void showFolderUsers();
    void addFolderUser(QStringList loginList);
    void removeFolderUser(QStringList loginList);

signals:
    void finished();
public slots:
    void aboutToQuitApp();
    void quit();
};

#endif // CLIENT_H
