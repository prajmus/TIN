#ifndef MESSAGE_H
#define MESSAGE_H

#include "opcodes.h"

#include <QTcpSocket>

class Message
{
public:
    Message(QTcpSocket *, quint8, QString, QString, bool, quint16 port = 0);
    QTcpSocket *sender;
    quint8 opCode;
    QString str1;
    QString str2;
    quint16 port;
    bool isComplicated;

};

#endif // MESSAGE_H
