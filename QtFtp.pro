#-------------------------------------------------
#
# Project created by QtCreator 2015-12-30T22:18:10
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtFtp
TEMPLATE = app


SOURCES += main.cpp\
        qtftp.cpp \
    ftpthread.cpp

HEADERS  += qtftp.h \
    ftpthread.h \
    config.h

FORMS    += qtftp.ui
