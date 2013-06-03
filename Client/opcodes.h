#ifndef OPCODES_H
#define OPCODES_H

const quint8 REGISTER = 0;
const quint8 REGISTER_SUCCESSFUL = 1;
const quint8 REGISTER_FAILED = 2;
const quint8 LOGIN = 10;
const quint8 LOGIN_SUCCESSFUL = 11;
const quint8 LOGIN_FAILED = 12;
const quint8 NEW_FILE = 20;
const quint8 PUSH_FILE = 21;
const quint8 FILE_ADDED = 22;
const quint8 DELETE_FILE = 30;
const quint8 FILE_DELETED = 31;
const quint8 MODIFY_FILE = 40;
const quint8 FILE_CHANGED = 41;
const quint8 LIST_FILES = 50;
const quint8 FILES_LISTING = 51;
const quint8 DISCONNECTING = 60;

#endif // OPCODES_H
