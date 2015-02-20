#include "mainwindow.h"
#include "global.h"
#include "application.h"
#include "Attachments/ChordPatternAttachment/chordpattern.h"


int main(int argc, char *argv[])
{
    Application a(argc, argv);

    a.fileIndex().restore();


    // load stylesheet
//    QFile styleSheetFile(":/style/style.css");
//    assert( styleSheetFile.open(QIODevice::ReadOnly) );
//    a.setStyleSheet( styleSheetFile.readAll() );

    MainWindow m;
    m.show();

    return a.exec();
}
