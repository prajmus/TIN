
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
            //Client.terminateClient();
            return;

        }

        // Rejestracja uzytkownika
        else if (command == "register") {
            //Client.connectToServer();
            std::cout << "Wprowadz login: ";
            getLine(login);
            while(true/*!Client.loginAvailable(login)*/) {
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
                    //Client.registerUser(login,password);
                    std::cout << "Konto zostalo zarejestrowane na serwerze.\n";
                    return;
                }
            }
        }
        else if (command == "status") {
            //Client.showStatus();
        }
        else if (command == "list") {
            //Client.listMonitoredFiles();
        }
        else if (command == "manage") {
            //Client.showManageUsage();
        }
        else if (command == "manage.list") {
            //Client.showFolderUsers();
        }
        else if (command == "manage.add") {
            //Client.addFolderUser();
        }
        else if (command == "manage.remove") {
            //Client.removeFolderUser();
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
