#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QCoreApplication>
#include <QString>
#include <QStringList>

#include "paths.h"

class Client
{

private:
    QCoreApplication *app;
    QString path;           // monitored by client

protected:
    Client();

public:
    void run();
    static Client& getInstance();

    QStringList* readConfigFile(QString configPath = "config");
    bool loginAvailable(QString login);
    void createAccount();
    void registerUser(QString login, QString password);
    void connectToServer();
    void terminateClient();
    bool compareLocalCopies(QString path = "localList");
    void synchronizeFiles();

    void showStatus();
    void showMonitoredFiles();


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
