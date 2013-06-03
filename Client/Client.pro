#-------------------------------------------------
#
# Project created by QtCreator 2013-05-31T00:20:32
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Client
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    client.cpp \
    utilities.cpp \
    CommandParser.cpp \
    filesystemwatcher.cpp \
    fileserver.cpp \
    comparator.cpp \
    file.cpp

HEADERS += \
    client.h \
    utilities.h \
    CommandParser.h \
    filesystemwatcher.h \
    fileserver.h \
    comparator.h \
    file.h \
    paths.h \
    opcodes.h
