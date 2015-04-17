#ifndef PDFCREATOR_H
#define PDFCREATOR_H

#include "Attachments/attachment.h"
#include "Attachments/pdfattachment.h"
#include "Attachments/ChordPatternAttachment/chordpatternattachment.h"
#include "Attachments/audioattachment.h"

#include "Database/SongDatabase/song.h"
#include "Database/EventDatabase/setlist.h"
#include "configurable.h"

#include "pdfpaintdevice.h"



//class QPdfWriter;
//class PDFCreator
//{


//public:
//    PDFCreator();
//    static bool paintSetlist( const Setlist* setlist, const QString & filename );
//    static const double UNIT;

//private:
//    static void paintAttachment( const Attachment* attachment, QPainter& painter );
//    static void paintSong(const Song* song ,QPainter& painter);

//    static void paintTitle(const Setlist *setlist, QPainter &painter );
//    static void paintSongTitle(const Song* song, QPainter& painter );

//    static void paintPDFAttachment( const PDFAttachment* attachment, QPainter& painter );
//    static void paintAudioAttachment( const AudioAttachment* attachment, QPainter& painter );
//    static void paintChordPatternAttachment( const ChordPatternAttachment* attachment, QPainter& painter );

//    static double scale();
//    static double aspectRatio();
//    static QRectF line( double top, double bottom );

//    static QPdfWriter* m_pdfWriter;
//    static QPdfWriter* pdf();
//    static void nextPage();

//    static void paintTableOfContents( const Setlist* setlist );
//};

class PDFCreator
{
    DECL_CONFIG( PDFCreator );
public:
    PDFCreator( const Setlist* setlist );
    ~PDFCreator();

    void save( const QString & filename );

private: // METHODES
    void paintSetlist( );
    void paintSong( const Song* song );
    void paintSongTitle( const Song* song );
    void paintAttachment( const Attachment* attachment );
    void paintPDFAttachment(const PDFAttachment* attachment );
    void paintAudioAttachment(const AudioAttachment* attachment);
    void paintChordPatternAttachment(const ChordPatternAttachment* attachment);
    void paintTitle();
    QPainter& painter();
    void nextPage();
    QRectF pageRect() const { return QRectF(QPointF(), m_pdfPainter->size()); }

private: // MEMBERS
    PDFPaintDevice* m_pdfPainter = NULL;
    const Setlist* const m_setlist;

private: // STATIC MEMBERS and METHODES
    friend QString labelSetlist( const Setlist* setlist );
};

#endif // PDFCREATOR_H
