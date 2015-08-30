#-------------------------------------------------
#
# Project created by QtCreator 2015-01-22T22:41:48
#
#-------------------------------------------------


#1//TODO pdfattachment seems to be rasterized before written in pdf.
#       this is slow and stupid for obvious reasons.

#2//TODO audio-attachment must be rewritten. The Index-based solution is not good imo.
#  it would be better to rely on filenames, with a user-dependend prefix (e.g. C:\Music\, /dev/media/music-volume/, ...)

#3//TODO decode aiff-audio-files.
#4//TODO increase the number of endings that can be indexed and played. Probably, this issue is gone when #2 is solved.
#5//TODO improve the overall gui.



QT       += core gui multimedia

QT += testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CAN2
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x
QMAKE_LFLAGS += -static-libgcc

unix {
LIBS += -lgit2
LIBS += -L/usr/local/lib -lpoppler-qt5
LIBS += -lavformat -lavcodec -lavutil -lpthread
LIBS += -lSoundTouch
}

win32 {
INCLUDEPATH += ../../libav-10.6-win32/win32/usr/include/
LIBS += -L../../bin -lgit2
LIBS += -L../../lib -lpoppler-qt5
LIBS += -L../../bin -lavformat -lavcodec -lavutil
LIBS += -L../../bin -lsoundtouch

}



SOURCES += main.cpp\
        mainwindow.cpp \
    configurable.cpp \
    Project/project.cpp \
    Database/SongDatabase/songdatabase.cpp \
    Database/EventDatabase/eventdatabase.cpp \
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
    Commands/AttachmentCommands/abstractchordpatternattachmenttransposecommand.cpp \
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
    Commands/SetlistCommands/setlistcommand.cpp \
    Commands/SetlistCommands/setlistremoveitemcommand.cpp \
    DatabaseView/EventDatabaseView/setlistitemselector.cpp \
    Commands/SetlistCommands/setlisteditdatacommand.cpp \
    Commands/SetlistCommands/setlistinsertitemcommand.cpp \
    DatabaseView/EventDatabaseView/setlistview.cpp \
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
    combobox.cpp \
    AttachmentView/IndexedFileAttachmentView/indexedfilechoosedialog.cpp \
    Commands/SongDatabaseCommands/songdatabasetoggleattributevisibility.cpp \
    Dialogs/tagdialog.cpp \
    Commands/edittagscommand.cpp \
    PDFCreator/page.cpp \
    PDFCreator/pdfcreator.cpp \
    AttachmentView/looselines.cpp \
    Dialogs/chordpatternviewer.cpp \
    PDFCreator/orphantsetlist.cpp \
    Dialogs/hud.cpp \
    Dialogs/addfilestoindexdialog.cpp \
    Project/zipped.cpp \
    Project/conflictfile.cpp \
    Project/gitrepository.cpp \
    Project/identity.cpp \
    Project/gitutil.cpp \
    Project/file.cpp \
    AttachmentView/chordpatternproxyattachmentview.cpp \
    Attachments/ChordPatternAttachment/chordpatternproxyattachment.cpp \
    Attachments/ChordPatternAttachment/abstractchordpatternattachment.cpp \
    Dialogs/exportpdfdialog.cpp \
    Dialogs/tageditor.cpp \
    Dialogs/chordpatternviewerscrollarea.cpp \
    DatabaseView/SongDatabaseView/CellEditors/durationeditor.cpp \
    programdialog.cpp \
    program.cpp \
    Commands/SongCommands/songeditprogramcommand.cpp \
    midicontroller.cpp \
    Database/EventDatabase/setlistitem.cpp \
    Commands/SetlistCommands/setlistmoverowscommand.cpp


HEADERS  += mainwindow.h \
    Project/project.h \
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
    Commands/AttachmentCommands/abstractchordpatternattachmenttransposecommand.cpp \
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
    Commands/SetlistCommands/setlistcommand.h \
    Commands/SetlistCommands/setlistremoveitemcommand.h \
    DatabaseView/EventDatabaseView/setlistitemselector.h \
    Commands/SetlistCommands/setlisteditdatacommand.h \
    Commands/SetlistCommands/setlistinsertitemcommand.h \
    DatabaseView/EventDatabaseView/setlistview.h \
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
    Commands/SongDatabaseCommands/songdatabasetoggleattributevisibility.h \
    Dialogs/tagdialog.h \
    Commands/edittagscommand.h \
    PDFCreator/page.h \
    PDFCreator/pdfcreatorconfig.h \
    PDFCreator/pdfcreator.h \
    poppler.h \
    soundtouch.h \
    poppler.h \
    soundtouch.h \
    AttachmentView/looselines.h \
    Dialogs/chordpatternviewer.h \
    PDFCreator/orphantsetlist.h \
    Dialogs/hud.h \
    Dialogs/addfilestoindexdialog.h \
    Project/zipped.h \
    Project/conflictfile.h \
    Project/gitrepository.h \
    Project/identity.h \
    Project/gitutil.h \
    git.h \
    Project/file.h \
    AttachmentView/chordpatternproxyattachmentview.h \
    Attachments/ChordPatternAttachment/chordpatternproxyattachment.h \
    Attachments/ChordPatternAttachment/abstractchordpatternattachment.h \
    Dialogs/exportpdfdialog.h \
    Dialogs/tageditor.h \
    Dialogs/chordpatternviewerscrollarea.h \
    DatabaseView/SongDatabaseView/CellEditors/durationeditor.h \
    programdialog.h \
    program.h \
    Commands/SongCommands/songeditprogramcommand.h \
    midicontroller.h \
    Database/databasemimedata.h \
    Database/databasesortproxy.h \
    Database/EventDatabase/setlistitem.h \
    Commands/SetlistCommands/setlistmoverowscommand.h



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
    Dialogs/commitdialog.ui \
    Dialogs/identitydialog.ui \
    Dialogs/configurationdialog.ui \
    DatabaseView/EventDatabaseView/eventdatabasewidget.ui \
    Dialogs/datetimedialog.ui \
    DatabaseView/EventDatabaseView/eventtableviewcontainer.ui \
    DatabaseView/EventDatabaseView/setlistwidget.ui \
    AttachmentView/IndexedFileAttachmentView/AudioAttachmentView/audioslider.ui \
    AttachmentView/IndexedFileAttachmentView/indexedfilechoosedialog.ui \
    Dialogs/tagdialog.ui \
    Dialogs/chordpatternviewer.ui \
    Dialogs/addfilestoindexdialog.ui \
    conflicteditor.ui \
    AttachmentView/chordpatternproxyattachmentview.ui \
    Dialogs/exportpdfdialog.ui \
    Dialogs/tageditor.ui \
    programdialog.ui


RESOURCES += \
    ressources.qrc

RC_FILE = can.rc


TRANSLATIONS += \
    can2_en_US.ts \
    can2_de_DE.ts
