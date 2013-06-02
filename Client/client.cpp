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
    loggedIn = false;
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
   // QStringList* config = readConfigFile(_CONFIG_PATH);
//    QStringList* config = readConfigFile();

    /*
    qDebug() << "Wypisanie listy:";
    for(int i = 0; i < config -> size(); i++) {
        qDebug() << config->at(i);
    }
    //*/

    /*
    this->path = config->at(0);
//    qDebug() << path;
    if(this->path != "")
        FileServer::getInstance().construct(this->path);
    else
        qDebug() << "Empty path!";
    //*/

    // Compare monitored folder with local file list
//    compareLocalCopies(QString path);

    // Synchronize changes with server
//    synchronizeFiles();
}

// Looks for config file and saves contents to list of configs
QStringList* Client::readConfigFile(QFile & file)
{
    //QFile file(configPath);
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

// Compares local copies of files with copies on server
// returns true when all files are up to date
// returns false when files need to be updated
bool Client::compareLocalCopies()
{
    QList< std::pair<QString,QDateTime> >* remoteList = getRemoteList();
    QStringList localList = FileServer::getInstance().getFileList();
//    QStringList toUpload;
//    QStringList toDownload;
//    QStringList toModify;

    for (int i=0; i<remoteList->size(); i++) {
        for (int j=0; j<localList.size(); j++) {
            if(remoteList->at(i).first == localList.at(j)) {
//                if();

            }
        }
    }
    return true;
}

// Gets list of files with their modification data from server
QList< std::pair<QString,QDateTime> >* Client::getRemoteList()
{
    QList< std::pair<QString,QDateTime> > * list = new QList<std::pair<QString,QDateTime> >();
    return list;
}

// Connects client to server
void Client::connectToServer()
{
    std::cout << "Connected to server\n";
}

void Client::logToServer() {
    std::cout << "Login: " << login.toStdString() << "\n";
    std::cout << "Password: " << password.toStdString() << "\n";
    loggedIn = true;
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

//
void Client::logIn()
{
    if (loggedIn)
        return;
    QFile file("config");
    if(!file.exists()) {
        qDebug() << "File doesn't exist!";
        QString ans;
        std::cout << "Czy masz juz konto? (tak/nie)\n";
        getLine(ans);
        if(ans == "tak") {
            std::cout << "Podaj sciezke do monitorowania.\n";
            getLine(path);
            std::cout << "Wprowadz login: ";
            getLine(login);
            std::cout << "Wprowadz haslo: ";
            getLine(password);
            logToServer();
            if(loggedIn) {
                createConfigFile();
            }
        }
        else if (ans == "nie"){
            createAccount();
        }
    }
    else {
        QStringList * config = readConfigFile(file);

        qDebug() << "Wypisanie listy:";

        int size = config -> size();

        for(int i = 0; i < size; i++) {
            qDebug() << config->at(i);
        }

        this -> path = config -> at(0);
        qDebug() << path;

        if (size == 1) {
            //login i haslo do podania
            std::cout << "Wprowadz login: ";
            getLine(login);
            std::cout << "Wprowadz haslo: ";
            getLine(password);
            logToServer();
        }
        else if (size == 2) {
            //podac tylko haslo
            login = config -> at(1);
            std::cout << "Wprowadz haslo: ";
            getLine(password);
            logToServer();
        }
        else {
            login = config -> at(1);
            password = config -> at(2);
            logToServer();
        }
    }
    if(loggedIn) {
        if(this -> path != "")
            FileServer::getInstance().construct(this -> path);
        else
         qDebug() << "Empty path!";
    }
}

// Creating new account
void Client::createAccount() {
    if(loggedIn)
        return;
    QString login;
    QString password;
    QString password2;

    std::cout << "Podaj sciezke do monitorowania.\n";
    getLine(path);
    std::cout << "Wprowadz login: ";
    getLine(login);
    while(! Client::getInstance().loginAvailable(login)) {
        std::cout << "Login zajety! Wprowadz nowy login: ";
        getLine(login);
        break;
    }
    this -> login = login;
    while(true) {
        std::cout << "Wprowadz haslo: ";
        getLine(password);
        std::cout << "Wprowadz ponownie haslo: ";
        getLine(password2);
        if ( password != password2 ) {
            std::cout << "Hasla sie nie zgadzaja!\n";
        } else {
            this -> password = password;
            Client::getInstance().registerUser(login,password);
            if(loggedIn) {
                createConfigFile();
                std::cout << "Konto zostalo zarejestrowane na serwerze.\n";
                if(this -> path != "")
                    FileServer::getInstance().construct(this -> path);
                else
                    qDebug() << "Empty path!";
                return;
            }
            else {
                return;
            }
        }
    }
}

void Client::createConfigFile() {
    QFile newFile("config");
    if (!newFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        std::cout << "Cos poszlo nie tak.\n";
        return;
    }

    QTextStream out(&newFile);
    out << path << "\n";
    out << login << "\n";
    out << password << "\n";
    newFile.close();
}

// Registering new user on server - sending a message (?) to register him
void Client::registerUser(QString login, QString password)
{
    std::cout << "User regisitered\n";
    //TODO:
    //prosba do serwera o zapisanie nowego uzytkownika
    //jezeli sie udalo ti wywolanie logToServer()
    //jezeli nie to jakies info o tym i prosze sprobowac pozniej
    logToServer();
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
