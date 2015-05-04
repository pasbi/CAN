#include "mainwindow.h"
#include "application.h"


int main(int argc, char *argv[])
{
    Application app(argc, argv);

    app.fileIndex().restore();

    MainWindow m;
    app.setMainWindow( &m );
    m.show();
    return app.exec();



}
