#-------------------------------------------------
#
# Project created by QtCreator 2015-12-30T22:18:10
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtFtp
TEMPLATE = app


SOURCES += main.cpp\
        qtftp.cpp \
    ftpthread.cpp \
    takvim.cpp \
    vtthread.cpp \
    config.cpp \
    global.cpp

HEADERS  += qtftp.h \
    ftpthread.h \
    takvim.h \
    vtthread.h \
    config.h \
    global.h

FORMS    += qtftp.ui
