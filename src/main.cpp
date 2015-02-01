#include "mainwindow.h"
#include <QApplication>
#include "global.h"
#include "UnitTest/unittests.h"
#include <QWidget>
#include "FileIndex/fileindex.h"
#include "application.h"


int main(int argc, char *argv[])
{
    Application a(argc, argv);

    MainWindow m;
    m.show();

    return a.exec();
}
