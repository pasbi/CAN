#include "mainwindow.h"
#include "global.h"
#include "application.h"
#include "Attachments/ChordPatternAttachment/chordpattern.h"
#include <QMediaPlayer>
#include <QAudioProbe>


int main(int argc, char *argv[])
{
    Application a(argc, argv);

    a.fileIndex().restore();


    // load stylesheet
//    QFile styleSheetFile(":/style/style.css");
//    assert( styleSheetFile.open(QIODevice::ReadOnly) );
//    a.setStyleSheet( styleSheetFile.readAll() );

    MainWindow m;
    m.show();

//    QMediaPlayer player;
//    player.setMedia(QUrl::fromLocalFile("/media/Volume/Musik/Muse/Black Holes and Revelations/Soldiers Poem.mp3"));
//    player.setVolume(100);
//    player.media().
//    player.setPlaybackRate(1.0/2);
//    player.play();

    return a.exec();
}
