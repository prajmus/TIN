
#include "CommandParser.h"

#include <sstream>

CommandParser::CommandParser()
{

}

void CommandParser::run()
{
    while(std::getline(std::cin,command)) {
        std::stringstream input(command);
        std::string login;
        std::string password;
        std::string password2;


        // Pusta komenda
        if (command.size() == 0) {
            continue;
        }
        // Wyjscie z programu
        if (command == "exit" || command == "quit") {
            return;
        }

        // Rejestracja uzytkownika
        else if (command.substr(0,8) == "register") {
            //Client.connectToServer();
            std::cout << "Wprowadz login: ";
            std::getline(std::cin,login);
            //Server.checkLoginAvailability(login);
            while(true) {
                std::cout << "Wprowadz haslo: ";
                std::getline(std::cin,password);
                std::cout << "Wprowadz ponownie haslo: ";
                std::getline(std::cin,password2);
                if ( password != password2 ) {
                    std::cout << "Hasla sie nie zgadzaja!\n";
                } else {
                    //Server.register(login,password);
                    std::cout << "Konto zostalo zarejestrowane na serwerze.\n";
                    return;
                }
            }


        }
    }
}

CommandParser& CommandParser::getInstance()
{
    static CommandParser instance;
    return instance;
}
