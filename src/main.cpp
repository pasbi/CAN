#include "mainwindow.h"
#include <QApplication>
#include "global.h"
#include "UnitTest/unittests.h"

int main(int argc, char *argv[])
{
    int* s = new int();


    UnitTests::runTests();


}
