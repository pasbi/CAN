#-------------------------------------------------
#
# Project created by QtCreator 2015-01-17T18:20:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Gui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    repetoire.cpp \
    calendar.cpp

HEADERS  += mainwindow.h \
    repetoire.h \
    calendar.h

FORMS    += mainwindow.ui \
    repetoire.ui \
    calendar.ui
