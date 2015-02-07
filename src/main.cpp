#include "mainwindow.h"
#include "global.h"
#include "application.h"
#include "Attachments/ChordPatternAttachment/chordpattern.h"


int main(int argc, char *argv[])
{

    QString string =
            "C - D E\n"
            "Hello World\n"
            "Am C F F F X C\n"
            "Am C F F F X C\n"
            "\n"
            "Hello\n";
    qDebug() << string;
    ChordPattern pattern(string);

    qDebug() << pattern.toString(Chord::LowerCase);

//    Application a(argc, argv);

//    MainWindow m;
//    m.show();

//    return a.exec();
}
