#-------------------------------------------------
#
# Project created by QtCreator 2013-05-30T22:49:29
#
#-------------------------------------------------

QT       += core sql

QT       -= gui

TARGET = Server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    database.cpp \
    account.cpp \
    md5.cpp \
    filesystemwatcher.cpp \
    fileserver.cpp \
    fileinfo.cpp \
    accountserver.cpp

HEADERS += \
    database.h \
    account.h \
    md5.h \
    filesystemwatcher.h \
    fileserver.h \
    fileinfo.h \
    accountserver.h
