#ifndef PDFCREATOR_H
#define PDFCREATOR_H

#include "Attachments/attachment.h"
#include "Attachments/pdfattachment.h"
#include "Attachments/ChordPatternAttachment/chordpatternattachment.h"
#include "Attachments/AudioAttachment/audioattachment.h"

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
    void paintAttachment(Attachment *attachment );
    void paintPDFAttachment(PDFAttachment *attachment );
    void paintChordPatternAttachment(const ChordPatternAttachment* attachment);
    void paintTitle();
    void paintTableOfContents();
    void paintHeadline(const QString& label , const EnumerationNumber &number = EnumerationNumber::invalid() );
    double leftMargin() const { return 5; }
    double rightMargin() const { return 10; }
    double topMargin() { return 5 + m_additionalTopMargin; }
    double bottomMargin() const { return 5; }

    void decoratePageNumbers();

    /**
     * @brief insertTableOfContentsStub insert a stub at current page.
     *  table of contents will be inserted there.
     */
    void insertTableOfContentsStub();
    QPainter *painter();
    void nextPage();
    QRectF pageRect() const { return QRectF(QPointF(), m_pdfPainter->size()); }
    QRectF line( double y ) const { return QRectF(QPointF(0, y), QSizeF(m_pdfPainter->size().width(), 10)); }

private: // MEMBERS
    PDFPaintDevice* m_pdfPainter = NULL;
    const Setlist* const m_setlist;
    int m_tableOfContentsPage = -1;
    int m_tableOfContentsNumPages = 0;
    double m_additionalTopMargin = 0; // is set if e.g. a headline need additional space.
    TableOfContents m_tableOfContents;

private: // STATIC MEMBERS and METHODES
    friend QString labelSetlist( const Setlist* setlist );
};

#endif // PDFCREATOR_H
