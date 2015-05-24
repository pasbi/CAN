#include "mainwindow.h"
#include "application.h"
#include "Dialogs/tagdialog.h"
#include <QTranslator>
#include <QDebug>
#include "configurable.h"
#include <QMessageBox>


int main(int argc, char *argv[])
{
    Application app(argc, argv);
    QTranslator translator;

    QString filename = QString("can2_%1").arg(MainWindow::config["Language"].toString());
    if (!translator.load( filename ))
    {
        QMessageBox::warning( NULL,
                              QObject::tr("Cannot find translation file"),
                              QString(QObject::tr("Translation file %1.qm not found.\n"
                                                  "Using fallback-english which is not recommended.")).arg(filename),
                              QMessageBox::Ok,
                              QMessageBox::NoButton );
    }
    app.installTranslator( &translator );

    app.fileIndex().restore();

    MainWindow m;
    app.setMainWindow( &m );
    m.show();

    return app.exec();
}
