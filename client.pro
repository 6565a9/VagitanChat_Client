#-------------------------------------------------
#
# Project created by QtCreator 2018-03-13T05:43:29
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    QVagitanChatProtocol.cpp \
    login.cpp \
    register.cpp \
    util.cpp \
    IOThread.cpp

HEADERS  += mainwindow.h \
    QVagitanChatProtocol.h \
    login.h \
    register.h \
    util.h \
    IOThread.h

FORMS    += mainwindow.ui

RESOURCES += \
    icons.qrc
