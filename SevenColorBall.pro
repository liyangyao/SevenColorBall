#-------------------------------------------------
#
# Project created by QtCreator 2015-03-10T19:57:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SevenColorBall
TEMPLATE = app
DESTDIR = $$PWD/bin

SOURCES += src/main.cpp\
        src/mainform.cpp \
    src/findpath.cpp

HEADERS  += src/mainform.h \
    src/findpath.h

FORMS    += src/mainform.ui

RESOURCES += \
    src/resource.qrc
