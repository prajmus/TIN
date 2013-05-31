#ifndef CLIENT_H
#define CLIENT_H

#include <QString>

class Client
{

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
    void addFolderUser(QString login);
    void removeFolderUser(QString login);
};

#endif // CLIENT_H
