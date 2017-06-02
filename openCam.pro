#-------------------------------------------------
#
# Project created by QtCreator 2017-05-28T21:36:47
#
#-------------------------------------------------
TARGET = openCam
CONFIG   += console c++11
CONFIG   -= app_bundle qt

TEMPLATE = app


SOURCES += main.cpp

LIBS +=-L /usr/local/lib  \
     -lopencv_core -lopencv_videoio -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc\
     -lraspicam -lraspicam_cv

LIBS +=-L/opt/vc/lib -lmmal -lmmal_core -lmmal_util

LIBS +=-L/usr/lib/arm-linux-gnueabihf -lboost_system -lpthread

INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/include



