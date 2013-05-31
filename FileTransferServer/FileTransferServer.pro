#-------------------------------------------------
#
# Project created by QtCreator 2013-05-30T19:34:07
#
#-------------------------------------------------

QT       += core network

QT       -= gui

CONFIG += c++11

TARGET = FileTransferServer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    filetransferserver.cpp \
    serverclient.cpp

HEADERS += \
    filetransferserver.h \
    serverclient.h
