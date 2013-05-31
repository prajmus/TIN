
#include "CommandParser.h"

#include <sstream>
#include <iostream>
#include <QTextStream>


CommandParser::CommandParser()
{

}

void CommandParser::run()
{
    std::cout << "run\n";
    QTextStream in(stdin);
    command = "costam";
    QTextStream out(stdout);
    //while(std::getline(std::cin,command.toStdString())) {
    while(true) {
        //command = in.readLine();
       out << command << endl;



        continue;/*
        //std::cout << "obrot while()\n";
        //std::stringstream input(command.toStdString());
        QString login;
        QString password;
        QString password2;

        //std::cout << input.str() << std::endl;


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
            std::getline(std::cin,login.toStdString());
            while(!Client.loginAvailable(login)) {
                std::cout << "Login zajety! Wprowadz nowy login: ";
                std::getline(std::cin,login.toStdString());
            }
            while(true) {
                std::cout << "Wprowadz haslo: ";
                std::getline(std::cin,password.toStdString());
                std::cout << "Wprowadz ponownie haslo: ";
                std::getline(std::cin,password2.toStdString());
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
        }*/
    }
}

CommandParser& CommandParser::getInstance()
{
    static CommandParser instance;
    return instance;
}
