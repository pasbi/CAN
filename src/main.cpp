#include "mainwindow.h"
#include "global.h"
#include "application.h"

#define FILENAME "/media/Volume/Musik/Coldplay/X&Y/Fix You.mp3"

int main(int argc, char *argv[])
{
    Application a(argc, argv);

    a.fileIndex().restore();

    MainWindow m;
    m.show();

    return a.exec();
}
