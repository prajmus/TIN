#ifndef CLIENT_H
#define CLIENT_H

#include <QCoreApplication>
#include <QString>


class Client
{
private:
    QCoreApplication *app;

protected:
    Client();

public:
    void run();
    void quit();
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
};

#endif // CLIENT_H
