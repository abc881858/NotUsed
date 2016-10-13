#-------------------------------------------------
#
# Project created by QtCreator 2015-11-04T23:32:45
#
#-------------------------------------------------

QT       += core gui websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DotaCardServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

win32 {
QMAKE_CXXFLAGS+=/wd"4819"
}
