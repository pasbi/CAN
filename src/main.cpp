#include "mainwindow.h"
#include "global.h"
#include "application.h"

#define FILENAME "/media/Volume/Musik/Coldplay/X&Y/Fix You.mp3"

//#define TEST


#include "PDFCreator/pdfcreator.h"
#include "PDFCreator/tableofcontents.h"
#include "PDFCreator/picturepainter.h"
#include <QLabel>

int main(int argc, char *argv[])
{
    Application app(argc, argv);

    app.fileIndex().restore();

    MainWindow m;
#ifndef TEST
    m.show();
    return app.exec();
#else
    Q_UNUSED(m);
    PDFCreator p( app.project()->eventDatabase()->events()[1]->setlist() );
    p.save( "/home/pascal/test.pdf" );
    return 0;
#endif
}
