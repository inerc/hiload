#-------------------------------------------------
#
# Project created by QtCreator 2016-09-21T20:30:13
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = DZ1
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    server.cpp \
    tpool.cpp \
    worker.cpp \
    connection.cpp \
    requesthandler.cpp

HEADERS += \
    server.h \
    tpool.h \
    worker.h \
    connection.h \
    requesthandler.h
