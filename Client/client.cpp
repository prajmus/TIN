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

void Client::showStatus() {
    std::cout << "Status usera i katalogu.\n";
}

void Client::showMonitoredFiles() {
    std::cout << "List of monitored files:\n";
}


void Client::showManageUsage() {
    std::cout << "Manage help:\n";
}


void Client::showFolderUsers() {
    std::cout << "List of users with access to this folder:\n";
}

void Client::addFolderUser(QString login) {
    //TcpClient.addFolderUser(QString login);
    std::cout << "User ";// << login << " added to shared folder!\nTo list users, type manage.list\n";
}

void Client::removeFolderUser(QString login) {
    //TcpClient.removeFolderUser(QString login);
    std::cout << "User ";// << login << " removed from shared folder!\nTo list users, type manage.list\n";
}
