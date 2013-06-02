#include "client.h"
#include "utilities.h"
#include "CommandParser.h"
#include "fileserver.h"
#include <iostream>
#include <QThread>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QDebug>


Client::Client(){

}

// Main loop
void Client::run()
{
    // Starting CommandParser in new thread

    //CommandParser::getInstance().process();
    QThread* thread = new QThread();
    CommandParser::getInstance().moveToThread(thread);
    CommandParser::getInstance().connect(thread, SIGNAL(started()),SLOT(process()));
    thread->connect(&CommandParser::getInstance(),SIGNAL(finished()),SLOT(quit()));
    CommandParser::getInstance().connect(&CommandParser::getInstance(),SIGNAL(finished()),SLOT(deleteLater()));
    thread->connect(thread, SIGNAL(finished()),SLOT(deleteLater()));
    thread->start();

    // Opening config file
    QStringList* config = readConfigFile(_CONFIG_PATH);
//    QStringList* config = readConfigFile();

    qDebug() << "Wypisanie listy:";
    for(int i = 0; i < config -> size(); i++) {
        qDebug() << config->at(i);
    }

    this->path = config->at(0);
    qDebug() << path;
    if(this->path != "")
        FileServer::getInstance().construct(this->path);
    else
        qDebug() << "Empty path!";

    // Compare monitored folder with local file list
//    compareLocalCopies(QString path);

    // Synchronize changes with server
//    synchronizeFiles();
}

// Looks for config file and saves contents to list of configs
QStringList* Client::readConfigFile(QString configPath)
{
    QFile file(configPath);
    QStringList* list = new QStringList();
    //QString line;

    if(!file.exists()) {
        qDebug() << "File doesn't exist!";

        // TODO: Interakcja z uzytkownikiem - podanie sciezki do monitorowanego katalogu

        //return NULL;
    }
    else {
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Couldn't open file!";
            return NULL;
        }

        QTextStream qin(&file);

        while(!qin.atEnd()) {
            QString line = qin.readLine();
            if(line != "")
                list->push_back(line);
        }

        file.close();
    }

    return list;
}

// Compares local copies of files with local list of files
bool Client::compareLocalCopies(QString path)
{
    QDir* dir = new QDir(".");
    QStringList currentFiles = dir->entryList();
    QFile localList(path);
    QStringList* listedFiles = new QStringList();
    QString line;
    int i;
    if(!localList.exists() || !localList.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Couldn't open file! Maybe it doesn't exist.";
        return false;
    }
    QTextStream qin(&localList);

    while(!qin.atEnd()) {
        line = qin.readLine();
        //qDebug() << line;
        if(line != "")
            listedFiles->push_back(line);
    }

    localList.close();

    //TODO: porownanie listy plikow lokalnych z tym, co sie znajduje w katalogu
    //      i podjecie odpowiednich akcji
    for(int i=0; i<listedFiles->size(); i++) {
        // TODO
    }
    return true;
}

// Connects client to server
void Client::connectToServer()
{
    std::cout << "Connected to server\n";
}


// Gets remote list of user's files and synchronizes it with local copies.
// Starts a new thread to exchange files in it
void Client::synchronizeFiles()
{

}

// Terminate client's connection with server and inform server, that client may
// be deleted from active clients' list
void Client::terminateClient()
{
    std::cout << "Sending server signal to terminate connection with client.\n";
    quit();
}

// Checking whether login is already in use
bool Client::loginAvailable(QString login)
{
    std::cout << "Login available\n";
    return true;
}

// Creating new account
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

// Registering new user on server - sending a message (?) to register him
void Client::registerUser(QString login, QString password)
{
    std::cout << "User regisitered\n";
}

// Shows client's status, currently downloaded/uploaded files, current files in folder
// (and users sharing this folder)
void Client::showStatus()
{
    std::cout << "User's and catalogue's status.\n";
}

// Displays list of files in monitored folder
void Client::showMonitoredFiles()
{
    QTextStream qout(stdout);
    QStringList fileList = FileServer::getInstance().getFileList();
    if(!fileList.empty()) {
        qout << "List of monitored files:"<<endl;
        for(int i=0; i<fileList.size(); i++) {
            qout << fileList.at(i) << endl;
        }
    }
    else {
        qout << "No files are monitored!"<<endl;
    }
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
