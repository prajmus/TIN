#include "client.h"
#include "utilities.h"
#include "CommandParser.h"
#include "fileserver.h"
#include "message.h"
#include "networkqueue.h"
#include "messagequeue.h"
#include "opcodes.h"
#include "communicationclient.h"

#include <iostream>
#include <QThread>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QSharedPointer>
#include <QDateTime>
#include <QFileInfo>


Client::Client() {

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

QString Client::trimm(QString path)
{
    int i=0;
    while(true) {
        if(path.at(i) != '/')
            ++i;
        else
            break;
    }
    path.remove(0,i);
    return path;
}

void Client::refreshLocalFiles()
{
    qDebug() << "--------------------------REFRESH -------------------";
    QList<QFileInfo> oldFiles = FileServer::getInstance().getList();
    QList<QDateTime *> oldDates = FileServer::getInstance().getDates();
    FileServer::getInstance().construct(path);
    QList<QFileInfo> newFiles = FileServer::getInstance().getList();
    bool *used = new bool[newFiles.size()];
    for(int i=0;i<newFiles.size();++i)
        used[i] = false;
    for(int i=0;i<oldFiles.size();++i)
    {
        bool found = false;
        for(int j=0;j<newFiles.size();++j) {
            qDebug() << "old: " << oldFiles.at(i).filePath() << " new: " << newFiles.at(j).filePath();
            if(oldFiles.at(i).filePath() == newFiles.at(j).filePath()) {
                qDebug() << oldDates.at(i)->toString("hh:mm.ssh") << ' ' << newFiles.at(j).lastModified().toString("hh:mm.ssh");
                if(*(oldDates.at(i)) == newFiles.at(j).lastModified()) {
                    found = true;
                    used[j] = true;
                    break;
                }
                else {
                    qDebug() << "File changed: " << newFiles.at(j).filePath();
                    QString file = trimm(newFiles.at(j).filePath());
                    NetworkQueue::getInstance().addMessage(QSharedPointer<Message>(new Message(MODIFY_FILE, file,"",true)));
                    used[j] = true;
                    found = true;
                    break;
                }
            }
        }
        if(!found) {
            QString file = trimm(oldFiles.at(i).filePath());
            NetworkQueue::getInstance().addMessage(QSharedPointer<Message>(new Message(DELETE_FILE, file, "", true)));
        }
    }
    for(int i=0;i<newFiles.size();++i) {
        if(!used[i]) {
            qDebug() << " new file";
            QString file = trimm(newFiles.at(i).filePath());
            NetworkQueue::getInstance().addMessage(QSharedPointer<Message>(new Message(NEW_FILE, file, "", true)));
        }
    }
    delete used;
}

// Compares local copies of files with copies on server
// returns true when all files are up to date
// returns false when files need to be updated
bool Client::compareLocalCopies()
{
  QStringList localList = FileServer::getInstance().getFileList();
  QSharedPointer<Message> msg;
//  QStringList* toUpload = new QStringList();
//  QStringList* toDownload = new QStringList();
  QDateTime timeLocal;
  QDateTime timeRemote;
  bool* used = new bool[remoteList->size()];
  for(int i=0;i<remoteList->size();i++)
    used[i] = false;
  bool found = false;

  for (int i=0; i<localList.size(); i++) {
    found = false;
    QFileInfo* info = FileServer::getInstance().getFileInfo(localList.at(i));
    if(info==NULL)
      continue;
    timeLocal = info->lastModified();
    for (int j=0; j<remoteList->size(); j++) {
      if(used[j])
        continue;
      if(localList.at(i) == remoteList->at(j).first) {
          used[j]=true;
        found = true;
        timeRemote = remoteList->at(j).second;
        if(timeLocal == timeRemote) {
          break;
        }
        else {
          if(timeLocal > timeRemote) {
            QString file = trimm(localList.at(i));
            msg = QSharedPointer<Message>(new Message(MODIFY_FILE, file, "", true));
            NetworkQueue::getInstance().addMessage(msg);
          }
          else {
              QString file = trimm(localList.at(i));
            msg = QSharedPointer<Message>(new Message(REQ_FILE, file, "", true));
            NetworkQueue::getInstance().addMessage(msg);
          }
        }
      }
    }
    if(!found) {
        QString file = trimm(localList.at(i));
      msg = QSharedPointer<Message>(new Message(NEW_FILE, file, "", true));
      NetworkQueue::getInstance().addMessage(msg);
    }
  }
  for (int i=0; i<remoteList->size(); i++) {
    if ( used[i] == false ) {
        QString file = trimm(remoteList->at(i).first);
      msg = QSharedPointer<Message>(new Message(REQ_FILE, file, "", true));
      NetworkQueue::getInstance().addMessage(msg);
    }
  }
  return true;
}


// Connects client to server
void Client::connectToServer()
{
    std::cout << "Connected to server\n";
}

void Client::logToServer()
{
  QSharedPointer<Message> p = QSharedPointer<Message>(new Message(LOGIN, login, password, true));
  NetworkQueue::getInstance().addMessage(p);

  QEventLoop loop;

  loop.connect(&CommunicationClient::getInstance(), SIGNAL(loginSuccessful()), SLOT(quit()));
  loop.connect(&CommunicationClient::getInstance(), SIGNAL(loginFailed()), SLOT(quit()));

  loop.exec();
//  if(loggedIn)
//  NetworkQueue::getInstance().addMessage(QSharedPointer<Message>(new Message(LIST_FILES, "","",false)));

}

// Terminate client's connection with server and inform server, that client may
// be deleted from active clients' list
void Client::terminateClient()
{
    std::cout << "Sending server signal to terminate connection with client.\n";
    CommunicationClient::getInstance().closeConnection();
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
                //FileServer::getInstance().construct(path);
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
        if(this -> path != "") {
            FileServer::getInstance().construct(this -> path);
            NetworkQueue::getInstance().addMessage(QSharedPointer<Message>(new Message(LIST_FILES, "","",false)));
        }
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
    while(! loginAvailable(login)) {
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
            registerUser(login,password);
            if(loggedIn) {
                createConfigFile();
                NetworkQueue::getInstance().addMessage(QSharedPointer<Message>(new Message(LIST_FILES, "","",false)));
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


void Client::listCommands() {
    std::cout << "Available commands:" << std::endl;
    std::cout << "login" << std::endl;
    std::cout << "register" << std::endl;
    std::cout << "exit" << std::endl;
    std::cout << "quit" << std::endl;
    std::cout << "list" << std::endl;
    std::cout << "status" << std::endl;
    //std::cout << "manage" << std::endl;
    //std::cout << "manage.list" << std::endl;
    //std::cout << "manage.add" << std::endl;
    //std::cout << "manage.remove" << std::endl;
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

void Client::setLoggedIn(bool logged)
{
  loggedIn = logged;
}

void Client::setRemoteList(QList<std::pair<QString, QDateTime> > *list)
{
  remoteList = list;
}

QString Client::getPath() const
{
  return path;
}

// Registering new user on server - sending a message (?) to register him
void Client::registerUser(QString login, QString password)
{
    std::cout << "User regisitered\n";
    //TODO:
    //prosba do serwera o zapisanie nowego uzytkownika
    //jezeli sie udalo ti wywolanie logToServer()
    //jezeli nie to jakies info o tym i prosze sprobowac pozniej
    QSharedPointer<Message> p = QSharedPointer<Message>(new Message(REGISTER, login, password, true));
    NetworkQueue::getInstance().addMessage(p);

    QEventLoop loop;

    loop.connect(&CommunicationClient::getInstance(), SIGNAL(registerSuccessful()), SLOT(quit()));
    loop.connect(&CommunicationClient::getInstance(), SIGNAL(registerFailed()), SLOT(quit()));

    loop.exec();


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
    FileServer::getInstance().construct(path);
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
