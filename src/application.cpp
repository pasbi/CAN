#include "application.h"
#include "Project/project.h"
#include "global.h"
#include "mainwindow.h"
#include "Database/SongDatabase/song.h"
#include "Database/SongDatabase/songdatabasesortproxy.h"
#include "DatabaseView/SongDatabaseView/songtableview.h"
#include "FileIndex/fileindex.h"
#include <QDir>


Application::Application(int &argc, char **argv) :
    QApplication( argc, argv)
{
    setApplicationName("CAN");
    setOrganizationDomain("CAN Developers");
    initPreferences();
    m_preferences.restore();
}

Application::~Application()
{
    m_preferences.save();

    delete m_fileIndex;
    m_fileIndex = nullptr;
}

Application & app()
{
    return *static_cast<Application*>(QApplication::instance());
}

void Application::pushCommand(Command *command)
{
    m_project->pushCommand( command );
}

void Application::setProject(Project *project)
{
    m_project = project;
}

void Application::setMainWindow( MainWindow* mainWindow )
{
    m_mainWindow = mainWindow;
}

void Application::undo() const
{
    return project()->undo();
}

void Application::redo() const
{
    return project()->redo();
}

void Application::beginMacro(const QString &text)
{
    m_project->beginMacro( text );
}

void Application::endMacro()
{
    m_project->endMacro();
}

Attachment* Application::currentAttachment() const
{
    return mainWindow()->currentAttachment();
}

FileIndex* Application::fileIndex() const
{
    if (!m_fileIndex)
    {
        m_fileIndex = new FileIndex();
    }
    return m_fileIndex;
}

void Application::setPreference(const QString &key, const QVariant &value)
{
    m_preferences.setValue(key, value);
}

void Application::initPreferences()
{
    m_preferences.registerPreference( "EnharmonicPolicy", new Preference(1) );
    m_preferences.registerPreference( "MinorPolicy", new Preference(0) );
    m_preferences.registerPreference( "RecentProjects", new Preference(QStringList()) );
    m_preferences.registerPreference( "MaxRecentProjects", new Preference(5) );
    m_preferences.registerPreference( "RecentCopyIndexedFilesTargetDir", new Preference(QDir::homePath()) );
    m_preferences.registerPreference( "locale", new Preference(QLocale::system().name()) );
    m_preferences.registerPreference( "FileIndexDefaultPath", new Preference(QDir::homePath()) );
    m_preferences.registerPreference( "Volume", new Preference(100) );
    m_preferences.registerPreference( "Muted", new Preference(false) );
    m_preferences.registerPreference( "LineSpacing", new Preference(1.3) );
    m_preferences.registerPreference( "ChordLineSpacing", new Preference(1) );
    m_preferences.registerPreference( "DefaultPDFSavePath", new Preference(QDir::homePath()) );
    m_preferences.registerPreference( "ChordPatternViewZoom", new Preference(1) );
    m_preferences.registerPreference( "ChordPatternViewHLine", new Preference(true) );
    m_preferences.registerPreference( "ChordPatternViewTwoColumn", new Preference(true) );
    m_preferences.registerPreference( "FileIndexEndings", new Preference(QStringList() << "mp3" << "ogg" << "aif" << "aiff" << "pdf") );

    m_preferences.registerPreference( "Quality", new Preference(InterfaceOptions(InterfaceOptions::SpinBox, tr("PDF"), tr("Quality")), 72, 0, 100, 1, tr("dpi")));
    m_preferences.registerPreference( "enable_TitlePagePattern", new Preference(InterfaceOptions(InterfaceOptions::CheckBox, tr("PDFCreator"), tr("Title Page")), true));
    m_preferences.registerPreference( "PDFSize", new Preference(InterfaceOptions(InterfaceOptions::ComboBox, tr("PDFCreator"), tr("Page Size")),
                                                                QStringList()   <<  tr("A0")
                                                                                <<  tr("A1")
                                                                                <<  tr("A2")
                                                                                <<  tr("A3")
                                                                                <<  tr("A4")
                                                                                <<  tr("A5")
                                                                                <<  tr("A6")
                                                                                <<  tr("A7")
                                                                                <<  tr("A8")
                                                                                <<  tr("A9")
                                                                                <<  tr("B0")
                                                                                <<  tr("B1")
                                                                                <<  tr("B2")
                                                                                <<  tr("B3")
                                                                                <<  tr("B4")
                                                                                <<  tr("B5")
                                                                                <<  tr("B6")
                                                                                <<  tr("B7")
                                                                                <<  tr("B8")
                                                                                <<  tr("B9")
                                                                                <<  tr("B10")
                                                                                <<  tr("C5E")
                                                                                <<  tr("Comm10E")
                                                                                <<  tr("DLE")
                                                                                <<  tr("Executive")
                                                                                <<  tr("Folio")
                                                                                <<  tr("Ledger")
                                                                                <<  tr("Legal")
                                                                                <<  tr("Letter")
                                                                                <<  tr("Tabloid"),
                                                                4 ) );

    m_preferences.registerPreference( "AlignSongs", new Preference(InterfaceOptions(InterfaceOptions::ComboBox, tr("PDFCreator"), tr("Align Songs")),
                                                                   QStringList()  << tr("No alignment")
                                                                                  << tr("Odd pages")
                                                                                  << tr("Even pages")
                                                                                  << tr("Duplex")
                                                                                  << tr("Endless")
                                                                                  << tr("Separate pages"),
                                                                   4 ) );
    m_preferences.registerPreference( "enable_TableOfContentsPattern", new Preference(InterfaceOptions(InterfaceOptions::CheckBox, tr("PDFCreator"), tr("Table of Contents")),
                                                                                      true ) );

    m_preferences.registerPreference( "PageNumbers", new Preference(InterfaceOptions(InterfaceOptions::CheckBox, tr("PDFCreator"), tr("Page Numbers")),
                                                                                      true ) );
    m_preferences.registerPreference( "PDFResolution", new Preference(InterfaceOptions(InterfaceOptions::SpinBox, tr("PDFCreator"), tr("Resolution")),
                                                                                      72, 0, 400, 1, tr("dpi") ) );
    m_preferences.registerPreference( "ContiuneOnNextPageMark", new Preference(InterfaceOptions(InterfaceOptions::CheckBox, tr("PDFCreator"), tr("Show to-be-continued-hint")),
                                                                                      true ) );
    m_preferences.registerPreference( "Channel", new Preference(InterfaceOptions(InterfaceOptions::SpinBox, tr("Midi"), tr("Channel")),
                                                                                      0, 0, 8, 1, "", tr("Disable Midi") ) );
    m_preferences.registerPreference( "minZoom", new Preference(0.0) );
    m_preferences.registerPreference( "maxZoom", new Preference(0.75) );
    m_preferences.registerPreference( "View", new Preference(0) );
    m_preferences.registerPreference( "HideInactived", new Preference(false) );
    m_preferences.registerPreference( "durationFormat", new Preference(tr("mm:ss")) );
    m_preferences.registerPreference( "dateTimeFormat", new Preference(tr("MM/dd/yy hh:mm ap")) );

}
