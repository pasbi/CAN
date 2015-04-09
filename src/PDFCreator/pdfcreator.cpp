#include "pdfcreator.h"
#include <QPainter>
#include <QPdfWriter>
#include "poppler/qt5/poppler-qt5.h"

DEFN_CONFIG( PDFCreator, "PDF Export" );


QPdfWriter* PDFCreator::m_pdfWriter = NULL;
PDFCreator::PDFCreator()
{
}

void PDFCreator::nextPage()
{
    assert( m_pdfWriter );
    assert( m_pdfWriter->newPage() );
}

bool PDFCreator::paintSetlist(const Setlist *setlist, const QString &filename)
{
    QFileInfo fileInfo( filename );
    if (fileInfo.exists())
    {
        qWarning() << "file " << filename << " does not exist.";
        return false;
    }
    if (fileInfo.isWritable())
    {
        qWarning() << "file " << filename << " is not writable.";
    }

    QPdfWriter writer( filename );
    m_pdfWriter = &writer;
    QPainter painter( &writer );

    paintTitle( painter );
    for ( const Song* song : setlist->songs() )
    {
        paintSong( song, painter );
    }

    m_pdfWriter = NULL;
    return true;

}

void PDFCreator::paintSong(const Song *song, QPainter& painter )
{
    paintSongTitle( song, painter );
    for (const Attachment* attachment : song->attachments())
    {
        paintAttachment( attachment, painter );
    }
}

void PDFCreator::paintTitle(const QPainter &painter)
{

}

void PDFCreator::paintSongTitle(const Song *song, QPainter &painter)
{

}

void PDFCreator::paintAttachment(const Attachment *attachment, QPainter& painter )
{
    if (attachment->type() == PDFAttachment::TYPE)
    {
        paintPDFAttachment( qobject_assert_cast<const PDFAttachment*>(attachment), painter );
    }
    else if (attachment->type() == AudioAttachment::TYPE)
    {
        paintAudioAttachment( qobject_assert_cast<const AudioAttachment*>(attachment), painter );
    }
    else if (attachment->type() == ChordPatternAttachment::TYPE)
    {
        paintChordPatternAttachment( qobject_assert_cast<const ChordPatternAttachment*>(attachment), painter );
    }
}

void PDFCreator::paintPDFAttachment(const PDFAttachment* attachment, QPainter &painter)
{
    Poppler::Document* doc = attachment->document();
    if (doc)
    {
        for (int i = 0; i < doc->numPages(); ++i)
        {
            attachment->document()->page(i)->renderToPainter( &painter );
        }
    }
}

void PDFCreator::paintAudioAttachment( const AudioAttachment* attachment, QPainter& painter )
{

}

void PDFCreator::paintChordPatternAttachment( const ChordPatternAttachment* attachment, QPainter& painter )
{

}
