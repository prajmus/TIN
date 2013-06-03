#include "message.h"

Message::Message(quint8 op, QString st1, QString st2, bool complicated, quint16 port) :
  opCode(op), str1(st1), str2(st2), port(port), isComplicated(complicated)
{
}

void Message::return_msg(bool ok)
{
}
