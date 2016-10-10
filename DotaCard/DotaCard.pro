#-------------------------------------------------
#
# Project created by QtCreator 2015-10-18T23:01:38
#
#-------------------------------------------------

QT       += core gui websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# With C++11 support
CONFIG += c++11
CONFIG += warn_on

TARGET = DotaCard
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    card.cpp \
    roomscene.cpp \
    area.cpp \
    engine.cpp \
    net.cpp \
    rule.cpp

HEADERS  += mainwindow.h \
    card.h \
    roomscene.h \
    area.h \
    engine.h \
    net.h \
    rule.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc

#QMAKE_TARGET_COMPANY = Home
#QMAKE_TARGET_DESCRIPTION = Detailed Description
#QMAKE_TARGET_COPYRIGHT = CopyRight
#QMAKE_TARGET_PRODUCT = DotaCard
#RC_CODEPAGE
#RC_ICONS
#RC_LANG
#VERSION = 1.1.0
