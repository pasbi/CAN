#-------------------------------------------------
#
# Project created by QtCreator 2015-01-22T22:41:48
#
#-------------------------------------------------


#//TODO port to Windows
#//TODO what happens if network is required but down?
#//TODO pdfattachment seems to be rasterized before written in pdf.
#       this is slow and stupid for obvious reasons.

QT       += core gui multimedia

QT += testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CAN2
TEMPLATE = app

#CONFIG += debug
QMAKE_CXXFLAGS_DEBUG -= -O2
QMAKE_CXXFLAGS_DEBUG += -Og

unix {
#you must have libgit2, poppler, libav, libsoundtouch installed.
#checkout zipgit, build it and update the path.
INCLUDEPATH += ../../ZipGit/src/
LIBS += -L../../ZipGit/build-ZipGit-Desktop-Debug/ -lzipgit
LIBS += -lgit2
LIBS += -L/usr/local/lib -lpoppler-qt5
LIBS += -lavformat -lavcodec -lavutil -lpthread
LIBS += -lSoundTouch
}

win32 {
INCLUDEPATH += ../../
INCLUDEPATH += ../../libav-10.6-win32/win32/usr/include/
INCLUDEPATH += ../../ZipGit/src/
LIBS += -L../../ZipGit/build-ZipGit-Desktop_Qt_5_4_1_MinGW_32bit-Debug/debug/ -lzipgit
LIBS += -L../../poppler/bin -lfreetype6 -ljpeg-8 -lopenjpeg -lpng12 -lpoppler -lpoppler-qt5
LIBS += -L../../libav-10.6-win32/win32/usr/bin -lavformat-55 -lavcodec-55 -lavutil-53
LIBS += -L../../\soundtouch\source\build-soundtouch-Desktop_Qt_5_4_1_MinGW_32bit-Debug\debug -lsoundtouch
}


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
    AttachmentView/IndexedFileAttachmentView/pdfattachmentview.cpp \
    Attachments/pdfattachment.cpp \
    DatabaseView/SongDatabaseView/songdatabasewidget.cpp \
    AttachmentView/attachmentchooser.cpp \
    Commands/AttachmentCommands/attachmentcommand.cpp \
    Commands/AttachmentCommands/attachmentrenamecommand.cpp \
    Attachments/ChordPatternAttachment/chord.cpp \
    Dialogs/clonedialog.cpp \
    Commands/AttachmentCommands/chordpatternattachmenttransposecommand.cpp \
    Dialogs/addfileindexsourcedialog.cpp \
    Dialogs/stringdialog.cpp \
    Commands/SongCommands/songduplicateattachmentcommand.cpp \
    AttachmentView/IndexedFileAttachmentView/indexedfileattachmentview.cpp \
    AttachmentView/IndexedFileAttachmentView/AudioAttachmentView/audioattachmentview.cpp \
    Attachments/AudioAttachment/audioattachment.cpp \
    AttachmentView/chordpatternedit.cpp \
    conflicteditor.cpp \
    Dialogs/commitdialog.cpp \
    identitymanager.cpp \
    Dialogs/identitydialog.cpp \
    Dialogs/configurationdialog.cpp \
    Dialogs/configurationwidgets.cpp \
    Commands/AttachmentCommands/fileattachmentcommandsethashcommand.cpp \
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
    Database/EventDatabase/setlist.cpp \
    Commands/SetlistCommands/setlistadditemcommand.cpp \
    Commands/SetlistCommands/setlistcommand.cpp \
    Commands/SetlistCommands/setlistremoveitemcommand.cpp \
    DatabaseView/EventDatabaseView/setlistitemselector.cpp \
    Commands/SetlistCommands/setlisteditdatacommand.cpp \
    Commands/SetlistCommands/setlistinsertitemcommand.cpp \
    DatabaseView/EventDatabaseView/setlistview.cpp \
    Commands/SetlistCommands/setlistmoverowscommand.cpp \
    Commands/AttachmentCommands/chordpatternattachmenteditpatterncommand.cpp \
    Attachments/AudioAttachment/sectionsmodel.cpp \
    Commands/AttachmentCommands/AudioAttachmentCommands/editsectioncommand.cpp \
    Attachments/AudioAttachment/buffer.cpp \
    Attachments/AudioAttachment/player.cpp \
    Commands/AttachmentCommands/AudioAttachmentCommands/insertsectioncommand.cpp \
    Commands/AttachmentCommands/AudioAttachmentCommands/sectionsmodelcommand.cpp \
    AttachmentView/IndexedFileAttachmentView/AudioAttachmentView/audioslider.cpp \
    AttachmentView/IndexedFileAttachmentView/AudioAttachmentView/slider.cpp \
    Commands/AttachmentCommands/AudioAttachmentCommands/deletesectioncommand.cpp \
    DatabaseView/databaseview.cpp \
    combobox.cpp \
    AttachmentView/IndexedFileAttachmentView/indexedfilechoosedialog.cpp \
    Commands/SongDatabaseCommands/songdatabasemovesongcommand.cpp \
    Commands/EventDatabaseCommands/eventdatabasemoveeventcommand.cpp \
    Commands/SongDatabaseCommands/songdatabasetoggleattributevisibility.cpp \
    Dialogs/tagdialog.cpp \
    Commands/edittagscommand.cpp \
    PDFCreator/page.cpp \
    PDFCreator/pdfcreator.cpp

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
    AttachmentView/IndexedFileAttachmentView/pdfattachmentview.h \
    Attachments/pdfattachment.h \
    DatabaseView/SongDatabaseView/songdatabasewidget.h \
    AttachmentView/attachmentchooser.h \
    Commands/AttachmentCommands/attachmentcommand.h \
    Attachments/ChordPatternAttachment/chord.h \
    Dialogs/clonedialog.h \
    Commands/AttachmentCommands/chordpatternattachmenttransposecommand.h \
    Dialogs/addfileindexsourcedialog.h \
    Dialogs/stringdialog.h \
    Commands/SongCommands/songduplicateattachmentcommand.h \
    AttachmentView/IndexedFileAttachmentView/indexedfileattachmentview.h \
    AttachmentView/IndexedFileAttachmentView/AudioAttachmentView/audioattachmentview.h \
    Attachments/AudioAttachment/audioattachment.h \
    AttachmentView/chordpatternedit.h \
    conflicteditor.h \
    global.h \
    Dialogs/commitdialog.h \
    identitymanager.h \
    Dialogs/identitydialog.h \
    Dialogs/configurationdialog.h \
    Dialogs/configurationwidgets.h \
    Commands/AttachmentCommands/fileattachmentcommandsethashcommand.h \
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
    Commands/AttachmentCommands/chordpatternattachmenteditpatterncommand.h \
    Attachments/AudioAttachment/sectionsmodel.h \
    Commands/AttachmentCommands/AudioAttachmentCommands/editsectioncommand.h \
    Attachments/AudioAttachment/buffer.h \
    Attachments/AudioAttachment/player.h \
    Commands/AttachmentCommands/AudioAttachmentCommands/insertsectioncommand.h \
    Commands/AttachmentCommands/AudioAttachmentCommands/sectionsmodelcommand.h \
    AttachmentView/IndexedFileAttachmentView/AudioAttachmentView/audioslider.h \
    AttachmentView/IndexedFileAttachmentView/AudioAttachmentView/slider.h \
    Commands/AttachmentCommands/AudioAttachmentCommands/deletesectioncommand.h \
    DatabaseView/databaseview.h \
    combobox.h \
    AttachmentView/IndexedFileAttachmentView/indexedfilechoosedialog.h \
    Commands/SongDatabaseCommands/songdatabasemovesongcommand.h \
    Commands/EventDatabaseCommands/eventdatabasemoveeventcommand.h \
    Commands/SongDatabaseCommands/songdatabasetoggleattributevisibility.h \
    Dialogs/tagdialog.h \
    Commands/edittagscommand.h \
    PDFCreator/page.h \
    PDFCreator/pdfcreatorconfig.h \
    PDFCreator/pdfcreator.h \
    poppler.h \
    soundtouch.h \
    poppler.h \
    soundtouch.h

FORMS    += mainwindow.ui \
    DatabaseView/SongDatabaseView/songtableviewcontainer.ui \
    filterwidget.ui \
    AttachmentView/chordpatternattachmentview.ui \
    AttachmentView/IndexedFileAttachmentView/pdfattachmentview.ui \
    DatabaseView/SongDatabaseView/songdatabasewidget.ui \
    AttachmentView/attachmentchooser.ui \
    Dialogs/clonedialog.ui \
    Dialogs/stringdialog.ui \
    AttachmentView/IndexedFileAttachmentView/indexedfileattachmentview.ui \
    AttachmentView/IndexedFileAttachmentView/AudioAttachmentView/audioattachmentview.ui \
    conflicteditor.ui \
    Dialogs/commitdialog.ui \
    Dialogs/identitydialog.ui \
    Dialogs/configurationdialog.ui \
    DatabaseView/EventDatabaseView/eventdatabasewidget.ui \
    Dialogs/datetimedialog.ui \
    DatabaseView/EventDatabaseView/eventtableviewcontainer.ui \
    DatabaseView/EventDatabaseView/setlistwidget.ui \
    AttachmentView/IndexedFileAttachmentView/AudioAttachmentView/audioslider.ui \
    AttachmentView/IndexedFileAttachmentView/indexedfilechoosedialog.ui \
    Dialogs/tagdialog.ui

OTHER_FILES += \
    FileIndexRecycle.txt

RESOURCES += \
    ressources.qrc

TRANSLATIONS += \
    can2_en_US.ts \
    can2_de_DE.ts
