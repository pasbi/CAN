#include "pdfcreator.h"
#include <QPainter>
//#include <QPdfWriter>
//#include "poppler/qt5/poppler-qt5.h"
//#include "Database/SongDatabase/songdatabase.h"
#include "Database/EventDatabase/event.h"

#define pt / m_pdfPainter.scale()


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
                       0,
                       ConfigurationItemOptions::ComboBoxOptions( QStringList() << "manyNiceFormats" ));

QMap<QString, QString> init_dictionary()
{
    QMap<QString, QString> dict;
    dict.insert("--", QString(QChar(0x2014)));
    return dict;
}
const QMap<QString, QString> PDFCreator::DICTIONARY = init_dictionary();

QString labelSetlist( const Setlist* setlist )
{
    QString title = PDFCreator::config["PDFTitlePattern"].toString();
    QString format = PDFCreator::config["DateTimeFormat"].toString();
    title.replace( "{EventTitle}", setlist->event()->label());
    title.replace( "{Begin}", setlist->event()->beginning().toString(format) );
    title.replace( "{End}",   setlist->event()->ending().toString(format) );

    for (const QString & key : PDFCreator::DICTIONARY.keys())
    {
        title.replace(key, PDFCreator::DICTIONARY[key]);
    }
    return title;
}

PDFCreator::PDFCreator( const Setlist *setlist ) :
    m_setlist( setlist )
{
    QString title = labelSetlist( m_setlist );
    QPdfWriter::PageSize pageSize = (QPdfWriter::PageSize) config["PDFSize"].toInt();
    m_pdfPainter = new PDFPaintDevice( title, pageSize );

    paintSetlist();
}



PDFCreator::~PDFCreator()
{
    delete m_pdfPainter;
    m_pdfPainter = NULL;
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

void paintTitle( const Setlist* setlist, QPainter& painter )
{
    QString title = labelSetlist( setlist );
    QRectF rect = QRectF( 0, 0, 100, 100 );
    painter.drawText( rect, Qt::AlignCenter, title );
    painter.drawRect( rect );
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

    paintTitle( m_setlist, painter() );

//    for ( const Song* song : setlist->songs() )
//    {
//        paintSong( song, painter );
//    }

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



//void PDFCreator::paintSong(const Song *song, QPainter& painter )
//{
//    paintSongTitle( song, painter );
//    for (const Attachment* attachment : song->attachments())
//    {
//        paintAttachment( attachment, painter );
//    }
//}

//QString labelSong( const Song* song )
//{
//    QString pattern = PDFCreator::config["SongTitlePattern"].toString();
//    for (int i = 0; i < song->database()->attributeKeys().length(); ++i)
//    {
//        QString attribute = song->attribute(i).toString();
//        QString key = song->database()->attributeKeys()[i];
//        key = SongDatabase::extractHeaderLabel( key );
//        pattern = pattern.replace( QString("{%1}").arg(key), attribute );
//    }
//    for (const QString & key : PDFCreator::DICTIONARY.keys())
//    {
//        pattern.replace(key, PDFCreator::DICTIONARY[key]);
//    }
//    return pattern;
//}

//void PDFCreator::paintSongTitle(const Song *song, QPainter &painter)
//{
//    painter.save();
//    QFont font = painter.font();
//    font.setPointSizeF( 30 pt );
//    painter.setFont( font );
//    if (config["SongTitle"].toBool())
//    {
//        painter.drawText( line( 20, 50 ),
//                          Qt::AlignCenter,
//                          labelSong( song ) );
//    }
//    painter.restore();
//}

//void PDFCreator::paintAttachment(const Attachment *attachment, QPainter& painter )
//{
//    if (attachment->type() == PDFAttachment::TYPE)
//    {
//        paintPDFAttachment( qobject_assert_cast<const PDFAttachment*>(attachment), painter );
//    }
//    else if (attachment->type() == AudioAttachment::TYPE)
//    {
//        paintAudioAttachment( qobject_assert_cast<const AudioAttachment*>(attachment), painter );
//    }
//    else if (attachment->type() == ChordPatternAttachment::TYPE)
//    {
//        paintChordPatternAttachment( qobject_assert_cast<const ChordPatternAttachment*>(attachment), painter );
//    }
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









//void PDFCreator::paintPDFAttachment(const PDFAttachment* attachment, QPainter &painter)
//{
//    Poppler::Document* doc = attachment->document();
//    if (doc)
//    {
//        for (int i = 0; i < doc->numPages(); ++i)
//        {
//            attachment->document()->page(i)->renderToPainter( &painter );
//        }
//    }
//}

//void PDFCreator::paintAudioAttachment( const AudioAttachment* attachment, QPainter& painter )
//{

//}

//void PDFCreator::paintChordPatternAttachment( const ChordPatternAttachment* attachment, QPainter& painter )
//{

//}


