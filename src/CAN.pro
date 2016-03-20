#-------------------------------------------------
#
# Project created by QtCreator 2015-01-22T22:41:48
#
#-------------------------------------------------


#1//TODO pdfattachment seems to be rasterized before written in pdf.
#       this is slow and stupid for obvious reasons.

#//TODO crash when indexing

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CAN
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x
QMAKE_LFLAGS += -static-libgcc

INCLUDEPATH += ../../Preferences/src
unix {
    LIBS += -L/usr/local/lib -lpoppler-qt5
    LIBS += -lavformat -lavcodec -lavutil -lpthread
    LIBS += -lSoundTouch
}

win32 {
    INCLUDEPATH += ../../libav-10.6-win32/win32/usr/include/
    LIBS += -L../../lib -lpoppler-qt5
    LIBS += -L../../bin -lavformat -lavcodec -lavutil
    LIBS += -L../../bin -lsoundtouch
}

CONFIG(debug, debug|release) {
    LIBS += -L../../Preferences/build-Preferences-Desktop-Debug/ -lPreferences
}
CONFIG(release, debug|release) {
    LIBS += -L../../Preferences/build-Preferences-Desktop-Release/ -lPreferences
}

SOURCES += main.cpp\
    mainwindow.cpp \
    Project/project.cpp \
    Database/SongDatabase/songdatabase.cpp \
    Database/EventDatabase/eventdatabase.cpp \
    Database/SongDatabase/song.cpp \
    Attachments/attachment.cpp \
    creatable.cpp \
    Attachments/ChordPatternAttachment/chordpatternattachment.cpp \
    Database/EventDatabase/event.cpp \
    Commands/command.cpp \
    DatabaseView/SongDatabaseView/songtableview.cpp \
    util.cpp \
    Database/SongDatabase/songdatabasesortproxy.cpp \
    filterwidget.cpp \
    FileIndex/fileindex.cpp \
    FileIndex/indexer.cpp \
    Attachments/indexedfileattachment.cpp \
    application.cpp \
    Commands/SongCommands/songremoveattachmentcommand.cpp \
    AttachmentView/attachmenteditor.cpp \
    AttachmentView/attachmentview.cpp \
    AttachmentView/chordpatternattachmentview.cpp \
    AttachmentView/IndexedFileAttachmentView/pdfattachmentview.cpp \
    Attachments/pdfattachment.cpp \
    DatabaseView/SongDatabaseView/songdatabasewidget.cpp \
    AttachmentView/attachmentchooser.cpp \
    Commands/AttachmentCommands/attachmentrenamecommand.cpp \
    chord.cpp \
    Commands/AttachmentCommands/abstractchordpatternattachmenttransposecommand.cpp \
    Dialogs/stringdialog.cpp \
    AttachmentView/IndexedFileAttachmentView/indexedfileattachmentview.cpp \
    AttachmentView/IndexedFileAttachmentView/AudioAttachmentView/audioattachmentview.cpp \
    Attachments/AudioAttachment/audioattachment.cpp \
    AttachmentView/chordpatternedit.cpp \
    Commands/AttachmentCommands/fileattachmentcommandsethashcommand.cpp \
    DatabaseView/EventDatabaseView/eventtableview.cpp \
    DatabaseView/EventDatabaseView/eventdatabasewidget.cpp \
    Dialogs/datetimedialog.cpp \
    Database/EventDatabase/eventdatabasesortproxy.cpp \
    DatabaseView/EventDatabaseView/setlistwidget.cpp \
    Database/EventDatabase/setlist.cpp \
    DatabaseView/EventDatabaseView/setlistitemselector.cpp \
    DatabaseView/EventDatabaseView/setlistview.cpp \
    Commands/AttachmentCommands/chordpatternattachmenteditpatterncommand.cpp \
    Attachments/AudioAttachment/sectionsmodel.cpp \
    Commands/AttachmentCommands/AudioAttachmentCommands/editsectioncommand.cpp \
    Attachments/AudioAttachment/buffer.cpp \
    Attachments/AudioAttachment/player.cpp \
    Commands/AttachmentCommands/AudioAttachmentCommands/insertsectioncommand.cpp \
    AttachmentView/IndexedFileAttachmentView/AudioAttachmentView/audioslider.cpp \
    AttachmentView/IndexedFileAttachmentView/AudioAttachmentView/slider.cpp \
    Commands/AttachmentCommands/AudioAttachmentCommands/deletesectioncommand.cpp \
    combobox.cpp \
    Dialogs/IndexedFileChooseDialog/indexedfilechoosedialog.cpp \
    PDFCreator/page.cpp \
    PDFCreator/pdfcreator.cpp \
    AttachmentView/looselines.cpp \
    Dialogs/chordpatternviewer.cpp \
    PDFCreator/orphantsetlist.cpp \
    AttachmentView/chordpatternproxyattachmentview.cpp \
    Attachments/ChordPatternAttachment/chordpatternproxyattachment.cpp \
    Attachments/ChordPatternAttachment/abstractchordpatternattachment.cpp \
    Dialogs/chordpatternviewerscrollarea.cpp \
    Dialogs/programdialog.cpp \
    Program/program.cpp \
    Commands/SongCommands/songeditprogramcommand.cpp \
    Program/midicontroller.cpp \
    Database/EventDatabase/setlistitem.cpp \
    Commands/SongCommands/songnewattachmentcommand.cpp \
    Commands/timerdecorator.cpp \
    Attachments/AudioAttachment/section.cpp \
    Program/midicommand.cpp \
    DatabaseView/databaseview.cpp \
    Database/databasesortproxy.cpp \
    DatabaseView/databasewidget.cpp \
    DatabaseView/ItemDelegates/setlistviewitemdelegate.cpp \
    Commands/SetlistCommands/setlistitemchangesongcommand.cpp \
    Database/EventDatabase/setlistproxy.cpp \
    Commands/DatabaseCommands/databaseeditcommand.cpp \
    Dialogs/IndexedFileChooseDialog/filefilter.cpp \
    DatabaseView/ItemDelegates/editablecomboboxdelegate.cpp \
    Dialogs/overlaylabel.cpp \
    overlaydecorator.cpp \
    AttachmentView/IndexedFileAttachmentView/AudioAttachmentView/volumeslider.cpp \
    DatabaseView/ItemDelegates/durationdelegate.cpp \
    Dialogs/copyindexedfilesdialog.cpp \
    AttachmentView/browseattachmentbuttonbox.cpp \
    playerwidget.cpp \
    DatabaseView/ItemDelegates/keydelegate.cpp \
    DatabaseView/ItemDelegates/durationeditor.cpp \
    DatabaseView/ItemDelegates/keyeditor.cpp \
    Dialogs/peoplesdialog.cpp \
    Dialogs/textdialog.cpp \
    DatabaseView/ItemDelegates/lineeditdelegate.cpp \
    global.cpp \
    DatabaseView/freezetableview.cpp \
    Merge/merge.cpp \
    Merge/mergedialog.cpp \
    Merge/databasemerger.cpp \
    Merge/mergelistwidget.cpp \
    Merge/mergeitem.cpp \
    Merge/mergelistwidgetitemwidget.cpp \
    Merge/combinedialogselectionmodel.cpp \
    Merge/mergelistwidgetselectionmodel.cpp \
    Database/databaseitem.cpp \
    optionpushbutton.cpp \
    Database/databasebase.cpp \
    Database/database.cpp \
    Database/SongDatabase/attachmentdatabase.cpp \
    Database/databaseitembase.cpp \
    Merge/databasemergerbase.cpp \
    Merge/AttachmentMergeWidgets/combineattachmentsdialog.cpp \
    Merge/AttachmentMergeWidgets/chordpatternattachmentmergewidget.cpp \
    Merge/AttachmentMergeWidgets/attachmentmergewidget.cpp \
    Merge/AttachmentMergeWidgets/attachmentmergewidgetbase.cpp \
    Merge/AttachmentMergeWidgets/indexedfileattachmentmergewidget.cpp \
    Merge/AttachmentMergeWidgets/audioattachmentmergewidget.cpp \
    Merge/AttachmentMergeWidgets/chordpatternproxyattachmentmergewidget.cpp \
    Merge/combinesongsdialog.cpp \
    Merge/combinedatabaseitemsdialog.cpp \
    Merge/combineeventsdialog.cpp \
    DatabaseView/ItemDelegates/setlistitemcombobox.cpp \
    switchwidget.cpp

HEADERS  += mainwindow.h \
    Project/project.h \
    Database/SongDatabase/songdatabase.h \
    Database/EventDatabase/eventdatabase.h \
    Database/database.h \
    Database/SongDatabase/song.h \
    Attachments/attachment.h \
    creatable.h \
    Attachments/ChordPatternAttachment/chordpatternattachment.h \
    Database/EventDatabase/event.h \
    Commands/command.h \
    DatabaseView/SongDatabaseView/songtableview.h \
    util.h \
    Database/SongDatabase/songdatabasesortproxy.h \
    filterwidget.h \
    FileIndex/fileindex.h \
    FileIndex/indexer.h \
    Attachments/indexedfileattachment.h \
    application.h \
    Commands/SongCommands/songremoveattachmentcommand.h \
    Commands/AttachmentCommands/attachmentrenamecommand.h \
    AttachmentView/attachmenteditor.h \
    AttachmentView/attachmentview.h \
    AttachmentView/chordpatternattachmentview.h \
    AttachmentView/IndexedFileAttachmentView/pdfattachmentview.h \
    Attachments/pdfattachment.h \
    DatabaseView/SongDatabaseView/songdatabasewidget.h \
    AttachmentView/attachmentchooser.h \
    chord.h \
    Commands/AttachmentCommands/abstractchordpatternattachmenttransposecommand.h \
    Dialogs/stringdialog.h \
    AttachmentView/IndexedFileAttachmentView/indexedfileattachmentview.h \
    AttachmentView/IndexedFileAttachmentView/AudioAttachmentView/audioattachmentview.h \
    Attachments/AudioAttachment/audioattachment.h \
    AttachmentView/chordpatternedit.h \
    global.h \
    Commands/AttachmentCommands/fileattachmentcommandsethashcommand.h \
    DatabaseView/EventDatabaseView/eventtableview.h \
    DatabaseView/EventDatabaseView/eventdatabasewidget.h \
    Dialogs/datetimedialog.h \
    Database/EventDatabase/eventdatabasesortproxy.h \
    DatabaseView/EventDatabaseView/setlistwidget.h \
    Database/EventDatabase/setlist.h \
    DatabaseView/EventDatabaseView/setlistitemselector.h \
    DatabaseView/EventDatabaseView/setlistview.h \
    Commands/AttachmentCommands/chordpatternattachmenteditpatterncommand.h \
    Attachments/AudioAttachment/sectionsmodel.h \
    Commands/AttachmentCommands/AudioAttachmentCommands/editsectioncommand.h \
    Attachments/AudioAttachment/buffer.h \
    Attachments/AudioAttachment/player.h \
    Commands/AttachmentCommands/AudioAttachmentCommands/insertsectioncommand.h \
    AttachmentView/IndexedFileAttachmentView/AudioAttachmentView/audioslider.h \
    AttachmentView/IndexedFileAttachmentView/AudioAttachmentView/slider.h \
    Commands/AttachmentCommands/AudioAttachmentCommands/deletesectioncommand.h \
    DatabaseView/databaseview.h \
    combobox.h \
    Dialogs/IndexedFileChooseDialog/indexedfilechoosedialog.h \
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
    AttachmentView/chordpatternproxyattachmentview.h \
    Attachments/ChordPatternAttachment/chordpatternproxyattachment.h \
    Attachments/ChordPatternAttachment/abstractchordpatternattachment.h \
    Dialogs/chordpatternviewerscrollarea.h \
    Dialogs/programdialog.h \
    Program/program.h \
    Commands/SongCommands/songeditprogramcommand.h \
    Program/midicontroller.h \
    Database/databasemimedata.h \
    Database/databasesortproxy.h \
    Database/EventDatabase/setlistitem.h \
    Commands/itemownercommanddecorator.h \
    Commands/SongCommands/songnewattachmentcommand.h \
    Commands/modelcommand.h \
    Commands/timerdecorator.h \
    Commands/DatabaseCommands/databasenewitemcommand.h \
    Commands/DatabaseCommands/databaseremoveitemcommand.h \
    Attachments/AudioAttachment/section.h \
    Program/midicommand.h \
    Database/databaseitem.h \
    Commands/DatabaseCommands/databasemoverowscommand.h \
    DatabaseView/ItemDelegates/typecomboboxdelegate.h \
    DatabaseView/ItemDelegates/lineeditdelegate.h \
    DatabaseView/ItemDelegates/itemdelegate.h \
    DatabaseView/databasewidget.h \
    DatabaseView/ItemDelegates/setlistviewitemdelegate.h \
    Commands/SetlistCommands/setlistitemchangesongcommand.h \
    DatabaseView/ItemDelegates/editablecomboboxdelegate.h \
    DatabaseView/ItemDelegates/durationdelegate.h \
    Database/EventDatabase/setlistproxy.h \
    Commands/DatabaseCommands/databaseeditcommand.h \
    Dialogs/IndexedFileChooseDialog/filefilter.h \
    Dialogs/overlaylabel.h \
    overlaydecorator.h \
    AttachmentView/IndexedFileAttachmentView/AudioAttachmentView/volumeslider.h \
    Dialogs/copyindexedfilesdialog.h \
    AttachmentView/browseattachmentbuttonbox.h \
    playerwidget.h \
    DatabaseView/ItemDelegates/keydelegate.h \
    DatabaseView/ItemDelegates/durationeditor.h \
    DatabaseView/ItemDelegates/keyeditor.h \
    Dialogs/peoplesdialog.h \
    Dialogs/textdialog.h \
    DatabaseView/freezetableview.h \
    Merge/merge.h \
    Merge/mergedialog.h \
    Merge/databasemerger.h \
    Database/SongDatabase/songdatabasetreemodel.h \
    Merge/mergelistwidget.h \
    Merge/mergeitem.h \
    Merge/mergelistwidgetitemwidget.h \
    Merge/combinedialogselectionmodel.h \
    Merge/mergelistwidgetselectionmodel.h \
    map.h \
    optionpushbutton.h \
    Database/databasebase.h \
    Database/SongDatabase/attachmentdatabase.h \
    Database/databaseitembase.h \
    Merge/databasemergerbase.h \
    Merge/AttachmentMergeWidgets/combineattachmentsdialog.h \
    Merge/AttachmentMergeWidgets/chordpatternattachmentmergewidget.h \
    Merge/AttachmentMergeWidgets/attachmentmergewidget.h \
    Merge/AttachmentMergeWidgets/attachmentmergewidgetbase.h \
    Merge/AttachmentMergeWidgets/indexedfileattachmentmergewidget.h \
    Merge/AttachmentMergeWidgets/audioattachmentmergewidget.h \
    Merge/AttachmentMergeWidgets/chordpatternproxyattachmentmergewidget.h \
    Merge/combinesongsdialog.h \
    Merge/combinedatabaseitemsdialog.h \
    Merge/combineeventsdialog.h \
    DatabaseView/ItemDelegates/setlistitemcombobox.h \
    switchwidget.h


FORMS    += mainwindow.ui \
    filterwidget.ui \
    AttachmentView/chordpatternattachmentview.ui \
    AttachmentView/IndexedFileAttachmentView/pdfattachmentview.ui \
    AttachmentView/attachmentchooser.ui \
    Dialogs/stringdialog.ui \
    AttachmentView/IndexedFileAttachmentView/indexedfileattachmentview.ui \
    AttachmentView/IndexedFileAttachmentView/AudioAttachmentView/audioattachmentview.ui \
    Dialogs/datetimedialog.ui \
    AttachmentView/IndexedFileAttachmentView/AudioAttachmentView/audioslider.ui \
    Dialogs/IndexedFileChooseDialog/indexedfilechoosedialog.ui \
    Dialogs/chordpatternviewer.ui \
    Dialogs/addfilestoindexdialog.ui \
    AttachmentView/chordpatternproxyattachmentview.ui \
    Dialogs/programdialog.ui \
    DatabaseView/EventDatabaseView/setlistwidget.ui \
    Dialogs/copyindexedfilesdialog.ui \
    AttachmentView/browseattachmentbuttonbox.ui \
    playerwidget.ui \
    Dialogs/peoplesdialog.ui \
    Dialogs/textdialog.ui \
    Merge/mergedialog.ui \
    Merge/combinedatabaseitemsdialog.ui \
    Merge/combineeventsdialog.ui


RESOURCES += \
    ressources.qrc

RC_FILE = can.rc


TRANSLATIONS += \
    can_en_US.ts \
    can_de_DE.ts
