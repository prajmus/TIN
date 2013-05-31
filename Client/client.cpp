#include "client.h"
#include <QTextStream>
#include <iostream>

Client::Client()
{
}

void Client::run()
{

}

Client& Client::getInstance()
{
    static Client instance;
    return instance;
}

void Client::connectToServer()
{
    std::cout << "Connected to server\n";
}

void Client::terminateClient()
{
    std::cout << "Sending server signal to terminate connection with client\n";
}

bool Client::loginAvailable(QString login)
{
    std::cout << "Login available\n";
    return true;
}

void Client::registerUser(QString login, QString password)
{
    std::cout << "User regisitered\n";
}

void Client::showStatus()
{
    std::cout << "Status usera i katalogu.\n";
}

void Client::showMonitoredFiles()
{
    std::cout << "List of monitored files:\n";
}


void Client::showManageUsage()
{
    std::cout << "Manage help:\n";
}


void Client::showFolderUsers()
{
    std::cout << "List of users with access to this folder:\n";
}

void Client::addFolderUser(QString login)
{
    //TcpClient.addFolderUser(QString login);
    std::cout << "User added\n";// << login << " added to shared folder!\nTo list users, type manage.list\n";
}

void Client::removeFolderUser(QString login)
{
    //TcpClient.removeFolderUser(QString login);
    std::cout << "User removed\n";// << login << " removed from shared folder!\nTo list users, type manage.list\n";
}
