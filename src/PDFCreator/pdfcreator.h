#ifndef PDFCREATOR_H
#define PDFCREATOR_H

#include "Attachments/attachment.h"
#include "Attachments/pdfattachment.h"
#include "Attachments/ChordPatternAttachment/chordpatternattachment.h"
#include "Attachments/audioattachment.h"

#include "Database/SongDatabase/song.h"
#include "Database/EventDatabase/setlist.h"
#include "configurable.h"



class QPdfWriter;
class PDFCreator
{

    DECL_CONFIG( PDFCreator );

public:
    PDFCreator();

private:
    static bool paintSetlist( const Setlist* setlist, const QString & filename );
    static void paintAttachment( const Attachment* attachment, QPainter& painter );
    static void paintSong(const Song* song ,QPainter& painter);

    static void paintTitle( const QPainter & painter );
    static void paintSongTitle( const Song* song, QPainter& painter );

    static void paintPDFAttachment( const PDFAttachment* attachment, QPainter& painter );
    static void paintAudioAttachment( const AudioAttachment* attachment, QPainter& painter );
    static void paintChordPatternAttachment( const ChordPatternAttachment* attachment, QPainter& painter );

    static QPdfWriter* m_pdfWriter;
    static void nextPage();


};

#endif // PDFCREATOR_H
