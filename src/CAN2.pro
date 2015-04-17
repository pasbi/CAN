#-------------------------------------------------
#
# Project created by QtCreator 2015-01-22T22:41:48
#
#-------------------------------------------------

QT       += core gui multimedia

QT += testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CAN2
TEMPLATE = app

CONFIG += debug
QMAKE_CXXFLAGS_DEBUG -= -O2
QMAKE_CXXFLAGS_DEBUG += -Og

LIBS += -L../../build-ZipGit-Desktop-Debug/ -lzipgit
LIBS += -lgit2
LIBS += -L/usr/local/lib -lpoppler-qt5


QMAKE_CXXFLAGS += -std=c++0x


SOURCES += main.cpp\
        mainwindow.cpp \
    configurable.cpp \
    project.cpp \
    Database/SongDatabase/songdatabase.cpp \
    Database/EventDatabase/eventdatabase.cpp \
    Database/database.cpp \
    taggable.cpp \
    Database/SongDatabase/song.cpp \
    Attachments/attachment.cpp \
    persistentobject.cpp \
    creatable.cpp \
    Attachments/ChordPatternAttachment/chordpatternattachment.cpp \
    Database/EventDatabase/event.cpp \
    UnitTest/creatabletest.cpp \
    UnitTest/unittests.cpp \
    Commands/command.cpp \
    Commands/SongDatabaseCommands/songdatabasenewsongcommand.cpp \
    DatabaseView/SongDatabaseView/songtableview.cpp \
    Commands/SongDatabaseCommands/songdatabaseeditsongcommand.cpp \
    Commands/SongDatabaseCommands/songdatabasecommand.cpp \
    Commands/SongDatabaseCommands/songdatabaseremovesongcommand.cpp \
    Commands/SongDatabaseCommands/songdatabasenewattributecommand.cpp \
    DatabaseView/SongDatabaseView/renamableheaderview.cpp \
    Commands/SongDatabaseCommands/songdatabaserenameheadercommand.cpp \
    Commands/SongDatabaseCommands/songdatabaseremovecolumncommand.cpp \
    util.cpp \
    DatabaseView/SongDatabaseView/songattributedelegate.cpp \
    DatabaseView/SongDatabaseView/CellEditors/celleditor.cpp \
    DatabaseView/SongDatabaseView/CellEditors/stringeditor.cpp \
    DatabaseView/SongDatabaseView/CellEditors/comboeditor.cpp \
    DatabaseView/SongDatabaseView/CellEditors/dateeditor.cpp \
    Database/SongDatabase/songdatabasesortproxy.cpp \
    DatabaseView/SongDatabaseView/songtableviewcontainer.cpp \
    filterwidget.cpp \
    progressdialog.cpp \
    FileIndex/fileindex.cpp \
    FileIndex/indexer.cpp \
    Attachments/indexedfileattachment.cpp \
    application.cpp \
    Commands/SongCommands/songaddattachmentcommand.cpp \
    Commands/SongCommands/songcommand.cpp \
    Commands/SongCommands/songremoveattachmentcommand.cpp \
    AttachmentView/attachmenteditor.cpp \
    AttachmentView/attachmentview.cpp \
    AttachmentView/chordpatternattachmentview.cpp \
    AttachmentView/pdfattachmentview.cpp \
    Attachments/pdfattachment.cpp \
    DatabaseView/SongDatabaseView/songdatabasewidget.cpp \
    AttachmentView/attachmentchooser.cpp \
    Commands/AttachmentCommands/attachmentcommand.cpp \
    Commands/AttachmentCommands/attachmentrenamecommand.cpp \
    Attachments/ChordPatternAttachment/chord.cpp \
    Attachments/ChordPatternAttachment/chordpattern.cpp \
    Dialogs/clonedialog.cpp \
    Commands/AttachmentCommands/chordpatternattachmenttransposecommand.cpp \
    Dialogs/addfileindexsourcedialog.cpp \
    Dialogs/stringdialog.cpp \
    Commands/SongCommands/songduplicateattachmentcommand.cpp \
    AttachmentView/indexedfileattachmentview.cpp \
    AttachmentView/audioattachmentview.cpp \
    Attachments/audioattachment.cpp \
    AttachmentView/chordpatternedit.cpp \
    advancedfilechooser.cpp \
    conflicteditor.cpp \
    advancedaudioplayer.cpp \
    Dialogs/commitdialog.cpp \
    identitymanager.cpp \
    Dialogs/identitydialog.cpp \
    Dialogs/configurationdialog.cpp \
    Dialogs/configurationwidgets.cpp \
    fileindexdialog.cpp \
    Dialogs/alternativefilemanager.cpp \
    Commands/AttachmentCommands/fileattachmentcommandsethashcommand.cpp \
    PDFCreator/pdfcreator.cpp \
    DatabaseView/EventDatabaseView/eventtableview.cpp \
    DatabaseView/EventDatabaseView/eventdatabasewidget.cpp \
    Dialogs/datetimedialog.cpp \
    Commands/EventDatabaseCommands/eventdatabaseneweventcommand.cpp \
    Commands/EventDatabaseCommands/eventdatabasecommand.cpp \
    Commands/EventDatabaseCommands/eventdatabaseremoveeventcommand.cpp \
    Database/EventDatabase/eventdatabasesortproxy.cpp \
    DatabaseView/EventDatabaseView/eventtableviewcontainer.cpp \
    DatabaseView/EventDatabaseView/setlistwidget.cpp \
    Commands/EventDatabaseCommands/eventdatabaseediteventcommand.cpp \
    commontypes.cpp \
    Database/EventDatabase/setlist.cpp \
    Commands/SetlistCommands/setlistadditemcommand.cpp \
    Commands/SetlistCommands/setlistcommand.cpp \
    Commands/SetlistCommands/setlistremoveitemcommand.cpp \
    DatabaseView/EventDatabaseView/setlistitemselector.cpp \
    Commands/SetlistCommands/setlisteditdatacommand.cpp \
    Commands/SetlistCommands/setlistinsertitemcommand.cpp \
    DatabaseView/EventDatabaseView/setlistview.cpp \
    Commands/SetlistCommands/setlistmoverowscommand.cpp \
    PDFCreator/pdfpaintdevice.cpp \
    PDFCreator/pdfpagesizewrapper.cpp \
    PDFCreator/tableofcontents.cpp

HEADERS  += mainwindow.h \
    project.h \
    configurable.h \
    Database/SongDatabase/songdatabase.h \
    Database/EventDatabase/eventdatabase.h \
    Database/database.h \
    taggable.h \
    Database/SongDatabase/song.h \
    Attachments/attachment.h \
    persistentobject.h \
    creatable.h \
    Attachments/ChordPatternAttachment/chordpatternattachment.h \
    Database/EventDatabase/event.h \
    UnitTest/creatabletest.h \
    UnitTest/unittests.h \
    Commands/command.h \
    Commands/SongDatabaseCommands/songdatabasenewsongcommand.h \
    DatabaseView/SongDatabaseView/songtableview.h \
    Commands/SongDatabaseCommands/songdatabaseeditsongcommand.h \
    Commands/SongDatabaseCommands/songdatabasecommand.h \
    Commands/SongDatabaseCommands/songdatabaseremovesongcommand.h \
    Commands/SongDatabaseCommands/songdatabasenewattributecommand.h \
    DatabaseView/SongDatabaseView/renamableheaderview.h \
    Commands/SongDatabaseCommands/songdatabaserenameheadercommand.h \
    Commands/SongDatabaseCommands/songdatabaseremovecolumncommand.h \
    util.h \
    DatabaseView/SongDatabaseView/songattributedelegate.h \
    DatabaseView/SongDatabaseView/CellEditors/celleditor.h \
    DatabaseView/SongDatabaseView/CellEditors/stringeditor.h \
    DatabaseView/SongDatabaseView/CellEditors/comboeditor.h \
    DatabaseView/SongDatabaseView/CellEditors/dateeditor.h \
    Database/SongDatabase/songdatabasesortproxy.h \
    DatabaseView/SongDatabaseView/songtableviewcontainer.h \
    filterwidget.h \
    progressdialog.h \
    FileIndex/fileindex.h \
    FileIndex/indexer.h \
    Attachments/indexedfileattachment.h \
    application.h \
    Commands/SongCommands/songaddattachmentcommand.h \
    Commands/SongCommands/songcommand.h \
    Commands/SongCommands/songremoveattachmentcommand.h \
    Commands/AttachmentCommands/attachmentrenamecommand.h \
    AttachmentView/attachmenteditor.h \
    AttachmentView/attachmentview.h \
    AttachmentView/chordpatternattachmentview.h \
    AttachmentView/pdfattachmentview.h \
    Attachments/pdfattachment.h \
    DatabaseView/SongDatabaseView/songdatabasewidget.h \
    AttachmentView/attachmentchooser.h \
    Commands/AttachmentCommands/attachmentcommand.h \
    Attachments/ChordPatternAttachment/chord.h \
    Attachments/ChordPatternAttachment/chordpattern.h \
    Dialogs/clonedialog.h \
    Commands/AttachmentCommands/chordpatternattachmenttransposecommand.h \
    Dialogs/addfileindexsourcedialog.h \
    Dialogs/stringdialog.h \
    Commands/SongCommands/songduplicateattachmentcommand.h \
    AttachmentView/indexedfileattachmentview.h \
    AttachmentView/audioattachmentview.h \
    Attachments/audioattachment.h \
    AttachmentView/chordpatternedit.h \
    advancedfilechooser.h \
    conflicteditor.h \
    advancedaudioplayer.h \
    global.h \
    Dialogs/commitdialog.h \
    identitymanager.h \
    Dialogs/identitydialog.h \
    Dialogs/configurationdialog.h \
    Dialogs/configurationwidgets.h \
    fileindexdialog.h \
    Dialogs/alternativefilemanager.h \
    Commands/AttachmentCommands/fileattachmentcommandsethashcommand.h \
    PDFCreator/pdfcreator.h \
    DatabaseView/EventDatabaseView/eventtableview.h \
    DatabaseView/EventDatabaseView/eventdatabasewidget.h \
    Dialogs/datetimedialog.h \
    Commands/EventDatabaseCommands/eventdatabaseneweventcommand.h \
    Commands/EventDatabaseCommands/eventdatabasecommand.h \
    Commands/EventDatabaseCommands/eventdatabaseremoveeventcommand.h \
    Database/EventDatabase/eventdatabasesortproxy.h \
    DatabaseView/EventDatabaseView/eventtableviewcontainer.h \
    DatabaseView/EventDatabaseView/setlistwidget.h \
    Commands/EventDatabaseCommands/eventdatabaseediteventcommand.h \
    commontypes.h \
    Database/EventDatabase/setlist.h \
    Commands/SetlistCommands/setlistadditemcommand.h \
    Commands/SetlistCommands/setlistcommand.h \
    Commands/SetlistCommands/setlistremoveitemcommand.h \
    DatabaseView/EventDatabaseView/setlistitemselector.h \
    Commands/SetlistCommands/setlisteditdatacommand.h \
    Commands/SetlistCommands/setlistinsertitemcommand.h \
    DatabaseView/EventDatabaseView/setlistview.h \
    Commands/SetlistCommands/setlistmoverowscommand.h \
    PDFCreator/pdfpaintdevice.h \
    PDFCreator/pdfpagesizewrapper.h \
    PDFCreator/tableofcontents.h

FORMS    += mainwindow.ui \
    DatabaseView/SongDatabaseView/songtableviewcontainer.ui \
    filterwidget.ui \
    progressdialog.ui \
    AttachmentView/chordpatternattachmentview.ui \
    AttachmentView/pdfattachmentview.ui \
    DatabaseView/SongDatabaseView/songdatabasewidget.ui \
    AttachmentView/attachmentchooser.ui \
    Dialogs/clonedialog.ui \
    Dialogs/stringdialog.ui \
    AttachmentView/indexedfileattachmentview.ui \
    advancedfilechooser.ui \
    AttachmentView/audioattachmentview.ui \
    conflicteditor.ui \
    Dialogs/commitdialog.ui \
    Dialogs/identitydialog.ui \
    Dialogs/configurationdialog.ui \
    fileindexdialog.ui \
    Dialogs/alternativefilemanager.ui \
    DatabaseView/EventDatabaseView/eventdatabasewidget.ui \
    Dialogs/datetimedialog.ui \
    DatabaseView/EventDatabaseView/eventtableviewcontainer.ui \
    DatabaseView/EventDatabaseView/setlistwidget.ui \
    DatabaseView/EventDatabaseView/setlistitemselector.ui

OTHER_FILES += \
    FileIndexRecycle.txt

RESOURCES += \
    ressources.qrc
