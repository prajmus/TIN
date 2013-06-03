#ifndef MESSAGE_H
#define MESSAGE_H

#include "opcodes.h"

class Message
{
public:
    Message(quint8, QString, QString, bool, quint16 port = 0);
    quint8 opCode;
    QString str1;
    QString str2;
    quint16 port;
    bool isComplicated;

    void return_msg(bool ok);
};

#endif // MESSAGE_H
