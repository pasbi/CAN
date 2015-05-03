#include "mainwindow.h"
#include "global.h"
#include "application.h"

//#define TEST


#include "Attachments/AudioAttachment/buffer.h"
#include <QAudioOutput>

int main(int argc, char *argv[])
{
    Application app(argc, argv);

    app.fileIndex().restore();

    MainWindow m;
    app.setMainWindow( &m );
#ifndef TEST
    m.show();
    return app.exec();
#else
    Q_UNUSED(m);
    PDFCreator p( app.project()->eventDatabase()->events()[1]->setlist() );
    p.save( "/home/pascal/test.pdf" );
    return 0;
#endif

//    Buffer buffer;
//    buffer.open( "/home/pascal/test.mp3" );

//    qDebug() << buffer.duration();
//    qDebug() << buffer.position();


//    buffer.seek( 22 );
//    qDebug() << buffer.position();

//    buffer.buffer().open( QIODevice::ReadOnly );
//    QAudioOutput out( QAudioDeviceInfo::defaultOutputDevice(), buffer.format() );


//    out.start( &buffer.buffer() );
////    out.setVolume( 50 );

//    return app.exec();



}
