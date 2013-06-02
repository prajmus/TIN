#include "client.h"
#include "CommandParser.h"
#include "utilities.h"

#include <sstream>
#include <iostream>
#include <QString>
#include <QStringList>

CommandParser::CommandParser(QObject *parent) : QObject(parent)
{

}

void CommandParser::process()
{
    QString command;
    QStringList list;

    while(true) {
        getLine(command);
        list = command.split(' ', QString::SkipEmptyParts);

        if (!list.empty()) {
            command = list.takeFirst();
        }
        else {
            std::cout << "pusta komenda\n";
            continue;
        }


        // Wyjscie z programu
        if (command == "exit" || command == "quit") {
            Client::getInstance().terminateClient();
            emit finished();
        }

        // Zalogowanie
        else if (command == "login") {
            Client::getInstance().logIn();
        }

        // Rejestracja uzytkownika
        else if (command == "register") {
            Client::getInstance().createAccount();
        }
        else if (command == "help") {
            Client::getInstance().listCommands();
        }
        else if (command == "status") {
            Client::getInstance().showStatus();
        }
        else if (command == "list") {
             Client::getInstance().showMonitoredFiles();
        }
        else if (command == "manage") {
             Client::getInstance().showManageUsage();
        }
        else if (command == "manage.list") {
             Client::getInstance().showFolderUsers();
        }
        else if (command == "manage.add") {
             Client::getInstance().addFolderUser(list);
        }
        else if (command == "manage.remove") {
             Client::getInstance().removeFolderUser(list);
        }
        else {
            std::cout << "I co teraz?\n";
        }
    }
}

CommandParser& CommandParser::getInstance()
{
    static CommandParser instance;
    return instance;
}
