#include "mainwindow.h"
#include "application.h"
#include "Dialogs/tagdialog.h"
#include <QTranslator>
#include "configurable.h"
#include <QMessageBox>
#include <QLibraryInfo>
#include "global.h"


int main(int argc, char *argv[])
{
    Application app(argc, argv);

    QString locale = MainWindow::config["locale"].toString();
    QLocale::setDefault( QLocale(locale) );
    QString language = QLocale::languageToString(QLocale(locale).language());

    QTranslator qtTranslator;
    QTranslator translator;

    bool useQtTranslator = (locale != "en_US");

    if ( ! (translator.load( QString("can2_%1").arg( locale ) )
            && (
                 !useQtTranslator
                 || qtTranslator.load(QString("qt_%1").arg( locale ),  QLibraryInfo::location(QLibraryInfo::TranslationsPath) )
               )
           )
       )
    {
        QMessageBox::warning( NULL,
                              QObject::tr("Cannot find translation files"),
                              QString(QObject::tr("Translation file \"%1\" not found.\n"
                                                  "Using fallback-english which is not recommended."))
                                    .arg( language ),
                              QMessageBox::Ok,
                              QMessageBox::NoButton );
    }
    else
    {
        app.installTranslator( &translator );
        if (useQtTranslator)
        {
            app.installTranslator( &qtTranslator );
        }
    }

    app.fileIndex().restore();

    MainWindow m;
    app.setMainWindow( &m );
    m.show();

    return app.exec();
}
