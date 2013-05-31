#include "client.h"
#include "CommandParser.h"
#include "utilities.h"

#include <sstream>
#include <iostream>

CommandParser::CommandParser()
{

}

void CommandParser::run()
{
    while(true) {
        getLine(command);
        // Pusta komenda
        if (command.size() == 0) {
            std::cout << "pusta komenda\n";
            continue;
        }

        // Wyjscie z programu
        if (command == "exit" || command == "quit") {
            Client::getInstance().terminateClient();
            return;
        }

        // Rejestracja uzytkownika
        else if (command == "register") {
            Client::getInstance().createAccount();
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
             Client::getInstance().addFolderUser(command);
        }
        else if (command == "manage.remove") {
             Client::getInstance().removeFolderUser(command);
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
