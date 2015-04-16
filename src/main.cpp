#include "mainwindow.h"
#include "global.h"
#include "application.h"
#include "PDFCreator/pdfcreator.h"

#define FILENAME "/media/Volume/Musik/Coldplay/X&Y/Fix You.mp3"

//#define TEST


int main(int argc, char *argv[])
{
    Application a(argc, argv);

    a.fileIndex().restore();

    MainWindow m;
#ifndef TEST
    m.show();
    return a.exec();
#else
    Q_UNUSED(m);
    PDFCreator p( a.project()->eventDatabase()->events()[1]->setlist() );
    p.save( "/home/pascal/test.pdf" );
    return 0;
#endif
}
