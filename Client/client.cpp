#include "client.h"
#include "utilities.h"
#include <QTextStream>
#include <iostream>

Client::Client()
{
}

void Client::run()
{

}

void Client::quit()
{
    emit finished();
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
    std::cout << "Sending server signal to terminate connection with client.\n";
    quit();
}

bool Client::loginAvailable(QString login)
{
    std::cout << "Login available\n";
    return true;
}

void createAccount() {
    QString login;
    QString password;
    QString password2;

    Client::getInstance().connectToServer();

    std::cout << "Wprowadz login: ";
    getLine(login);
    while(! Client::getInstance().loginAvailable(login)) {
        std::cout << "Login zajety! Wprowadz nowy login: ";
        getLine(login);
        break;
    }
    while(true) {
        std::cout << "Wprowadz haslo: ";
        getLine(password);
        std::cout << "Wprowadz ponownie haslo: ";
        getLine(password2);
        if ( password != password2 ) {
            std::cout << "Hasla sie nie zgadzaja!\n";
        } else {
            Client::getInstance().registerUser(login,password);
            std::cout << "Konto zostalo zarejestrowane na serwerze.\n";
            return;
        }
    }
}

void Client::registerUser(QString login, QString password)
{
    std::cout << "User regisitered\n";
}

void Client::showStatus()
{
    std::cout << "User's and catalogue's status.\n";
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

void Client::aboutToQuitApp()
{

}
