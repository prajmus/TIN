#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QCoreApplication>
#include <QString>
#include <QStringList>


class Client
{

private:
    QCoreApplication *app;

protected:
    Client();

public:
    void run();

    static Client& getInstance();

    void connectToServer();
    void terminateClient();
    bool loginAvailable(QString login);
    void createAccount();
    void registerUser(QString login, QString password);

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
