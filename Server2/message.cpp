#include "message.h"

Message::Message(QTcpSocket *socket, quint8 op, QString st1, QString st2, bool complicated, bool toOne, quint16 port) :
  opCode(op), str1(st1), str2(st2), port(port), isComplicated(complicated), sender(socket), toOne(toOne)
{
}

