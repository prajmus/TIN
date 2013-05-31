#include "client.h"
#include "utilities.h"
#include "CommandParser.h"
#include "fileserver.h"
#include <iostream>
#include <QThread>


Client::Client(){

}

void Client::run()
{
    //CommandParser::getInstance().process();
    QThread* thread = new QThread();
    CommandParser::getInstance().moveToThread(thread);
    CommandParser::getInstance().connect(thread, SIGNAL(started()),SLOT(process()));
    thread->connect(&CommandParser::getInstance(),SIGNAL(finished()),SLOT(quit()));
    CommandParser::getInstance().connect(&CommandParser::getInstance(),SIGNAL(finished()),SLOT(deleteLater()));
    thread->connect(thread, SIGNAL(finished()),SLOT(deleteLater()));
    thread->start();


     //CommandParser::getInstance().connect(thread, SIGNAL(quit()),SLOT(finished()));
    /*
    QObject::connect(thread, SIGNAL(started()), parser, SLOT(process()));
    QObject::connect(parser, SIGNAL(finished()), thread, SLOT(quit()));
    QObject::connect(parser, SIGNAL(finished()), parser, SLOT(deleteLater()));
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    */



}

void Client::quit()
{
    QCoreApplication::quit();
    //emit finished();
}

void Client::aboutToQuitApp()
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
    std::cout << "Sending server signal to terminate connection with client.\n";
    quit();
}

bool Client::loginAvailable(QString login)
{
    std::cout << "Login available\n";
    return true;
}

void Client::createAccount() {
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
    std::cout << FileServer::getInstance().getFileInfo(".").filePath();
}

void Client::showManageUsage()
{
    std::cout << "Manage help:\n";
}

void Client::showFolderUsers()
{
    std::cout << "List of users with access to this folder:\n";
}

void Client::addFolderUser(QStringList loginList)
{
    //TcpClient.addFolderUser(QString login);
    std::cout << "User added\n";// << login << " added to shared folder!\nTo list users, type manage.list\n";
}

void Client::removeFolderUser(QStringList loginList)
{
    //TcpClient.removeFolderUser(QString login);
    std::cout << "User removed\n";// << login << " removed from shared folder!\nTo list users, type manage.list\n";
}









/*Client::Client(QObject *parent) :
    QObject(parent)
{
    app = QCoreApplication::instance();
}*/

/*
void Client::aboutToQuitApp()
{

}*/
