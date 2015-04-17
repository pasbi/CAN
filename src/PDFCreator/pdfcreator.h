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
#include "tableofcontents.h"


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
    void paintTableOfContents();

    /**
     * @brief insertTableOfContentsStub insert a stub at current page.
     *  table of contents will be inserted there.
     */
    void insertTableOfContentsStub();
    QPainter& painter();
    void nextPage();
    QRectF pageRect() const { return QRectF(QPointF(), m_pdfPainter->size()); }

private: // MEMBERS
    PDFPaintDevice* m_pdfPainter = NULL;
    const Setlist* const m_setlist;
    int m_tableOfContentsPage = -1;
    TableOfContents m_tableOfContents;

private: // STATIC MEMBERS and METHODES
    friend QString labelSetlist( const Setlist* setlist );
};

#endif // PDFCREATOR_H
