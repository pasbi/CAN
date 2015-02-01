#include "mainwindow.h"
#include "global.h"
#include "application.h"


int main(int argc, char *argv[])
{
    Application a(argc, argv);

    MainWindow m;
    m.show();

    return a.exec();
}
