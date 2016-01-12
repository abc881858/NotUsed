#-------------------------------------------------
#
# Project created by QtCreator 2015-10-18T23:01:38
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DotaCard
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    card.cpp \
    roomscene.cpp \
    area.cpp

HEADERS  += mainwindow.h \
    card.h \
    roomscene.h \
    area.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
