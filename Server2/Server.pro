#-------------------------------------------------
#
# Project created by QtCreator 2013-05-30T22:49:29
#
#-------------------------------------------------

QT       += core sql network

QT       -= gui

TARGET = Server
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    database.cpp \
    account.cpp \
    md5.cpp \
    filesystemwatcher.cpp \
    fileserver.cpp \
    fileinfo.cpp \
    accountserver.cpp \
    serverclient.cpp \
    filetransferserver.cpp \
    communicationserver.cpp \
    networkqueue.cpp \
    messagequeue.cpp \
    message.cpp

HEADERS += \
    database.h \
    account.h \
    md5.h \
    filesystemwatcher.h \
    fileserver.h \
    fileinfo.h \
    accountserver.h \
    serverclient.h \
    filetransferserver.h \
    communicationserver.h \
    networkqueue.h \
    messagequeue.h \
    message.h
