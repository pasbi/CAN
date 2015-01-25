#include "mainwindow.h"
#include <QApplication>
#include "global.h"
#include "UnitTest/unittests.h"
#include <QWidget>

void init(void*& ptr)
{
    ptr = new QList<int>();
}

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    MainWindow m;
    m.show();

    return a.exec();
}
