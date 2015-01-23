#include "mainwindow.h"
#include <QApplication>
#include "creatable.h"
#include "Attachments/attachment.h"
#include "global.h"
#include "UnitTest/unittests.h"

int main(int argc, char *argv[])
{

    Attachment* cpa = 0;
    if (!CREATE("ChordPatternAttachment", cpa))
    {
        qDebug() << "false";
    }
    else
    {
        qDebug() << cpa->bark();
    }

    UnitTests::runTests();


}
