#include "message.h"

Message::Message(quint8 op, QString st1, QString st2, quint16 po, bool complicated) :
  opCode(op), str1(st1), str2(st2), port(po), isComplicated(complicated)
{
}

void Message::return_msg(bool ok)
{
}
