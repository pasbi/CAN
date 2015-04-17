#include "pdfcreator.h"
#include <QPainter>
//#include <QPdfWriter>
//#include "poppler/qt5/poppler-qt5.h"
#include "Database/SongDatabase/songdatabase.h"
#include "Database/EventDatabase/event.h"
#include "pdfpagesizewrapper.h"

#define pt / m_pdfPainter->scale()

DEFN_CONFIG( PDFCreator, "PDF Export" );

CONFIGURABLE_ADD_ITEM( PDFCreator,
                       PDFTitlePattern,
                       "Title",
                       "Setlist\n\n-- {EventTitle} --\n\n{Begin}",
                       ConfigurationItemOptions::TextEditOptions( TR("Pattern") ) );
CONFIGURABLE_ADD_ITEM( PDFCreator,
                       SongTitle,
                       "Song Title",
                       true,
                       ConfigurationItemOptions::CheckboxOptions() );
CONFIGURABLE_ADD_ITEM( PDFCreator,
                       SongTitlePattern,
                       "Song Title",
                       "{Title}\n-- {Artist} --",
                       ConfigurationItemOptions::TextEditOptions( TR("Pattern") ) );
CONFIGURABLE_ADD_ITEM( PDFCreator,
                       DateTimeFormat,
                       "Date time format",
                       TR("dd.MM.yyyy, hh:mm"),
                       ConfigurationItemOptions::LineEditOptions( TR("Date time pattern") ) );
CONFIGURABLE_ADD_ITEM( PDFCreator,
                       PDFSize,
                       "Page size",
                       pageSizeToInt( QPdfWriter::A4 ),
                       ConfigurationItemOptions::ComboBoxOptions( QStringList() <<  "A0"
                                                                                <<  "A1"
                                                                                <<  "A2"
                                                                                <<  "A3"
                                                                                <<  "A4"
                                                                                <<  "A5"
                                                                                <<  "A6"
                                                                                <<  "A7"
                                                                                <<  "A8"
                                                                                <<  "A9"
                                                                                <<  "B0"
                                                                                <<  "B1"
                                                                                <<  "B2"
                                                                                <<  "B3"
                                                                                <<  "B4"
                                                                                <<  "B5"
                                                                                <<  "B6"
                                                                                <<  "B7"
                                                                                <<  "B8"
                                                                                <<  "B9"
                                                                                <<  "B10"
                                                                                <<  "C5E"
                                                                                <<  "Comm10E"
                                                                                <<  "DLE"
                                                                                <<  "Executive"
                                                                                <<  "Folio"
                                                                                <<  "Ledger"
                                                                                <<  "Legal"
                                                                                <<  "Letter"
                                                                                <<  "Tabloid" ) );
//                                                                                <<  "Custom" ) ); // we do not support custom paper by now.


QMap<QString, QString> init_dictionary()
{
    QMap<QString, QString> dict;
    dict.insert("--", QString(QChar(0x2014)));
    return dict;
}

const QMap<QString, QString> DICTIONARY = init_dictionary();

QString labelSetlist( const Setlist* setlist )
{
    QString title = PDFCreator::config["PDFTitlePattern"].toString();
    QString format = PDFCreator::config["DateTimeFormat"].toString();
    title.replace( "{EventTitle}", setlist->event()->label());
    title.replace( "{Begin}", setlist->event()->beginning().toString(format) );
    title.replace( "{End}",   setlist->event()->ending().toString(format) );

    for (const QString & key : DICTIONARY.keys())
    {
        title.replace(key, DICTIONARY[key]);
    }
    return title;
}

PDFCreator::PDFCreator( const Setlist *setlist ) :
    m_setlist( setlist )
{
    QString title = labelSetlist( m_setlist );
    QPdfWriter::PageSize pageSize = intToPageSize( config["PDFSize"].toInt() );
    m_pdfPainter = new PDFPaintDevice( title, pageSize );

    paintSetlist();
}



PDFCreator::~PDFCreator()
{
    delete m_pdfPainter;
    m_pdfPainter = NULL;
}

void PDFCreator::nextPage()
{
    assert( m_pdfPainter );
    m_pdfPainter->nextPage();
}

QPainter& PDFCreator::painter()
{
    assert( m_pdfPainter );
    return m_pdfPainter->painter();
}

void PDFCreator::save(const QString &filename)
{
    m_pdfPainter->save( filename );
}

void PDFCreator::paintTitle()
{
    QString title = labelSetlist( m_setlist );
    painter().drawText( pageRect(), Qt::AlignCenter, title );
}

void PDFCreator::paintSetlist()
{

    m_pdfPainter->nextPage();
//    QFont font = painter.font();
//    font.setPointSizeF( font.pointSize() pt );
//    painter.scale( scale(), scale() );
//    painter.setFont( font );

//    if ( true )
//    {
//        paintTableOfContents( setlist );
//    }

    paintTitle();

    for ( const Song* song : m_setlist->songs() )
    {
        paintSong( song );
    }
}

void PDFCreator::paintSong(const Song *song)
{
    paintSongTitle( song );
    for (const Attachment* attachment : song->attachments())
    {
        paintAttachment( attachment );
    }
}

QString labelSong( const Song* song )
{
    QString pattern = PDFCreator::config["SongTitlePattern"].toString();
    for (int i = 0; i < song->database()->attributeKeys().length(); ++i)
    {
        QString attribute = song->attribute(i).toString();
        QString key = song->database()->attributeKeys()[i];
        key = SongDatabase::extractHeaderLabel( key );
        pattern = pattern.replace( QString("{%1}").arg(key), attribute );
    }
    for (const QString & key : DICTIONARY.keys())
    {
        pattern.replace(key, DICTIONARY[key]);
    }
    return pattern;
}

void PDFCreator::paintSongTitle(const Song *song)
{
    if (config["SongTitle"].toBool())
    {
        nextPage();
        painter().save();
        QFont font = painter().font();
        font.setPointSizeF( 30 pt );
        painter().setFont( font );
        painter().drawText( QRect(0, 0, 100, 100),
                            Qt::AlignCenter,
                            labelSong( song ) );
        painter().restore();
    }
}

bool printAttachment( const Attachment* attachment )
{
    Q_UNUSED(attachment);
    return true;    //TODO
}

void PDFCreator::paintAttachment(const Attachment *attachment )
{
    if (printAttachment( attachment ))
    {
        nextPage();
        if (attachment->type() == PDFAttachment::TYPE)
        {
            paintPDFAttachment( qobject_assert_cast<const PDFAttachment*>(attachment) );
        }
        else if (attachment->type() == AudioAttachment::TYPE)
        {
            paintAudioAttachment( qobject_assert_cast<const AudioAttachment*>(attachment) );
        }
        else if (attachment->type() == ChordPatternAttachment::TYPE)
        {
            paintChordPatternAttachment( qobject_assert_cast<const ChordPatternAttachment*>(attachment) );
        }
    }
}






//QPdfWriter* PDFCreator::m_pdfWriter = NULL;
//const double PDFCreator::UNIT = 100;    // do not change this. never.

//QPdfWriter* PDFCreator::pdf()
//{
//    assert( m_pdfWriter );
//    return m_pdfWriter;
//}

//double PDFCreator::scale()
//{
//    return pdf()->width() / UNIT;
//}

//double PDFCreator::aspectRatio()
//{
//    return (double) pdf()->height() / pdf()->width();
//}

//QRectF PDFCreator::line(double top, double bottom)
//{
//    return QRectF( 0, top, UNIT, bottom );
//}

//PDFCreator::PDFCreator()
//{
//}

//void PDFCreator::nextPage()
//{
//    assert( m_pdfWriter );
//    assert( m_pdfWriter->newPage() );
//}



//bool PDFCreator::paintSetlist(const Setlist *setlist, const QString &filename)


//}



//void PDFCreator::paintTableOfContents(const Setlist *setlist)
//{
//    typedef struct TOCItem {
//        TOCItem( const QString& label, const int level, const int page ) :
//            label( label ), level( level ), page( page )
//        { }
//        const QString label;
//        const int level;
//        const int page;
//    } TOCItem;

//    QList<TOCItem> items;

//    // gather items
//    for (const Song* song : setlist->songs())
//    {
////        items.append( TOCItem( labelSong( song ), 0,  ) );
//    }

//}




void PDFCreator::paintPDFAttachment(const PDFAttachment* attachment )
{
    Poppler::Document* doc = attachment->document();
    if (doc)
    {
        for (int i = 0; i < doc->numPages(); ++i)
        {
            attachment->document()->page(i)->renderToPainter( &painter() );
        }
    }
    painter().drawText( pageRect(), "PDF" );
}

void PDFCreator::paintAudioAttachment( const AudioAttachment* attachment )
{
    painter().drawText( pageRect(), "Audio" );
}

void PDFCreator::paintChordPatternAttachment( const ChordPatternAttachment* attachment )
{
    painter().drawText( pageRect(), "CPA" );
}


