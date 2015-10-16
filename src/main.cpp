#include <QTranslator>
#include <QMessageBox>
#include <QLibraryInfo>

#include "mainwindow.h"
#include "application.h"
#include "Dialogs/tagdialog.h"
#include "configurable.h"
#include "global.h"

#ifdef Q_OS_WIN32
#include <windows.h>
#endif


void installTranslator()
{
    QString localeCode = MainWindow::config["locale"].toString();
    QLocale locale( localeCode );
    QLocale::setDefault( locale );
    QString language = QLocale::languageToString( locale.language() );

    bool useQtTranslator = (localeCode != "en_US");

    QTranslator qtBaseTranslator;
    // load qtbase-translator
    if (useQtTranslator)
    {
        if (qtBaseTranslator.load(":/translations/qtbase_" + localeCode))
        {
            if (!Application::installTranslator( &qtBaseTranslator ))
            {
                qWarning() << "failed to install translator.";
            }
        }
        else
        {
            qWarning() << "failed to load translator qtbase.";
        }
    }

    // load qtmultimedia-translator
    QTranslator qtMultimediaTranslator;
    if (useQtTranslator)
    {
        if (qtMultimediaTranslator.load(":/translations/qtmultimedia_" + localeCode))
        {
            if (!Application::installTranslator( &qtMultimediaTranslator ))
            {
                qWarning() << "failed to install translator qtmultimedia.";
            }
        }
        else
        {
            qWarning() << "failed to load translator.";
        }
    }

    // load translator
    QTranslator translator;
    if (translator.load(":/translations/can2_" + localeCode))
    {
        if (!Application::installTranslator( &translator ))
        {
            qWarning() << "failed to install translator.";
        }
    }
    else
    {
        qWarning() << "cannot load translation file for " << language;
    }
}

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN32
    ShowWindow( GetConsoleWindow(), SW_HIDE );
#endif

    Application app( argc, argv );

    installTranslator();

    MainWindow m;
    m.show();

    return app.exec();
}

