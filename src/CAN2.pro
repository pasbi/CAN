#-------------------------------------------------
#
# Project created by QtCreator 2015-01-22T22:41:48
#
#-------------------------------------------------

QT       += core gui

QT += testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CAN2
TEMPLATE = app

LIBS += -L../../build-ZipGit-Desktop-Debug/ -lzipgit
LIBS += -L../../libgit2/build/ -lgit2

QMAKE_CXXFLAGS += -std=c++0x


SOURCES += main.cpp\
        mainwindow.cpp \
    project.cpp \
    Database/SongDatabase/songdatabase.cpp \
    Database/DateDatabase/datedatabase.cpp \
    Database/database.cpp \
    taggable.cpp \
    Database/SongDatabase/song.cpp \
    Attachments/attachment.cpp \
    persistentobject.cpp \
    creatable.cpp \
    Attachments/chordpatternattachment.cpp \
    fileindex.cpp \
    Database/DateDatabase/date.cpp \
    UnitTest/creatabletest.cpp \
    UnitTest/unittests.cpp \
    Commands/command.cpp \
    configurable.cpp \
    global.cpp \
    Commands/SongDatabaseCommands/songdatabasesetdatacommand.cpp \
    Commands/SongDatabaseCommands/songdatabasenewsongcommand.cpp \
    SongTableView/songtableview.cpp \
    Commands/SongDatabaseCommands/songdatabaseeditsongcommand.cpp \
    Commands/SongDatabaseCommands/songdatabasecommand.cpp \
    Commands/SongDatabaseCommands/songdatabaseremovesongcommand.cpp \
    Commands/SongDatabaseCommands/songdatabasenewattributecommand.cpp \
    SongTableView/renamableheaderview.cpp \
    Commands/SongDatabaseCommands/songdatabaserenameheadercommand.cpp

HEADERS  += mainwindow.h \
    project.h \
    Database/SongDatabase/songdatabase.h \
    Database/DateDatabase/datedatabase.h \
    Database/database.h \
    taggable.h \
    Database/SongDatabase/song.h \
    Attachments/attachment.h \
    persistentobject.h \
    creatable.h \
    Attachments/chordpatternattachment.h \
    global.h \
    fileindex.h \
    Database/DateDatabase/date.h \
    UnitTest/creatabletest.h \
    UnitTest/unittests.h \
    Commands/command.h \
    configurable.h \
    Commands/SongDatabaseCommands/songdatabasesetdatacommand.h \
    Commands/SongDatabaseCommands/songdatabasenewsongcommand.h \
    SongTableView/songtableview.h \
    Commands/SongDatabaseCommands/songdatabaseeditsongcommand.h \
    Commands/SongDatabaseCommands/songdatabasecommand.h \
    Commands/SongDatabaseCommands/songdatabaseremovesongcommand.h \
    Commands/SongDatabaseCommands/songdatabasenewattributecommand.h \
    SongTableView/renamableheaderview.h \
    Commands/SongDatabaseCommands/songdatabaserenameheadercommand.h

FORMS    += mainwindow.ui
