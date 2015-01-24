#include "mainwindow.h"
#include <QApplication>
#include "global.h"
#include "UnitTest/unittests.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow m;
    m.show();

    return a.exec();
}
