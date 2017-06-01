#-------------------------------------------------
#
# Project created by QtCreator 2017-05-28T20:14:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = raspiCam
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/local/Cellar/boost/1.64.0_1/include\
               /usr/local/include

LIBS += -L/usr/local/Cellar/boost/1.64.0_1/lib -lboost_system -lboost_thread-mt \
        -L/usr/local/lib  -lopencv_world -lopencv_contrib_world
