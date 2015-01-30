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
    FileIndex/fileindex.cpp \
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
    Commands/SongDatabaseCommands/songdatabaserenameheadercommand.cpp \
    Commands/SongDatabaseCommands/songdatabaseremovecolumncommand.cpp \
    util.cpp \
    SongTableView/songattributedelegate.cpp \
    SongTableView/CellEditors/celleditor.cpp \
    SongTableView/CellEditors/stringeditor.cpp \
    SongTableView/CellEditors/comboeditor.cpp \
    SongTableView/CellEditors/dateeditor.cpp \
    Database/SongDatabase/songdatabasesortproxy.cpp \
    SongTableView/songtableviewcontainer.cpp \
    FileIndexView/fileindexview.cpp \
    FileIndexView/fileindexviewcontainer.cpp \
    FileIndex/bihashedfileindex.cpp \
    filterwidget.cpp

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
    FileIndex/fileindex.h \
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
    Commands/SongDatabaseCommands/songdatabaserenameheadercommand.h \
    Commands/SongDatabaseCommands/songdatabaseremovecolumncommand.h \
    util.h \
    SongTableView/songattributedelegate.h \
    SongTableView/CellEditors/celleditor.h \
    SongTableView/CellEditors/stringeditor.h \
    SongTableView/CellEditors/comboeditor.h \
    SongTableView/CellEditors/dateeditor.h \
    Database/SongDatabase/songdatabasesortproxy.h \
    SongTableView/songtableviewcontainer.h \
    FileIndexView/fileindexviewcontainer.h \
    FileIndexView/fileindexview.h \
    FileIndex/bihashedfileindex.h \
    filterwidget.h

FORMS    += mainwindow.ui \
    SongTableView/songtableviewcontainer.ui \
    FileIndexView/fileindexviewcontainer.ui \
    filterwidget.ui

OTHER_FILES += \
    FileIndexRecycle.txt
