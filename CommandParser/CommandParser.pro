#-------------------------------------------------
#
# Project created by QtCreator 2013-05-30T22:36:04
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = CommandParser
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    CommandParser.cpp \
    utilities.cpp \
    ../Client/client.cpp

HEADERS += \
    CommandParser.h \
    utilities.h \
    ../Client/client.h
