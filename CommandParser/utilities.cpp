#include <iostream>
#include <string>
#include "utilities.h"

void getLine(QString & qstring)
{
    std::string str;
    getline(std::cin, str);
    qstring = str.c_str();
    return;
}
