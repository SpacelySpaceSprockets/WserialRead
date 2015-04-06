#-------------------------------------------------
#
# Project created by QtCreator 2015-03-23T18:30:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WserialRead
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    wterm.cpp

HEADERS  += mainwindow.h \
    wterm.h

FORMS    += mainwindow.ui

QT += serialport
QT += designer
