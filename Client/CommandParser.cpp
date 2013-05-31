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
    QString login;
    QString password;
    QString password2;

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
