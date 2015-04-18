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
                       "{Title} -- {Artist}",
                       ConfigurationItemOptions::LineEditOptions( TR("Pattern") ) );
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
CONFIGURABLE_ADD_ITEM( PDFCreator,
                       TableOfContents,
                       "Table of Contents",
                       true,
                       ConfigurationItemOptions::CheckboxOptions() );


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

    QFont font( "times" );
    font.setPointSizeF( 12 pt );
    m_pdfPainter->setDefaultFont( font );

    paintSetlist();
    decoratePageNumbers();
}



PDFCreator::~PDFCreator()
{
    delete m_pdfPainter;
    m_pdfPainter = NULL;
}

void PDFCreator::nextPage()
{
    m_additionalTopMargin = 0;
    assert( m_pdfPainter );
    m_pdfPainter->nextPage();
}

QPainter* PDFCreator::painter()
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
    QPainter* p = painter();
    p->drawText( pageRect(), Qt::AlignCenter, title );
}

void PDFCreator::insertTableOfContentsStub()
{
    m_tableOfContentsPage = m_pdfPainter->currentPageNumber() + 1;
}

void PDFCreator::paintSetlist()
{

    nextPage();
//    QFont font = painter.font();
//    font.setPointSizeF( font.pointSize() pt );
//    painter.scale( scale(), scale() );
//    painter.setFont( font );


    paintTitle();
    insertTableOfContentsStub();

    for ( const Song* song : m_setlist->songs() )
    {
        paintSong( song );
    }

    if ( true )
    {
        paintTableOfContents( );
    }
}

void PDFCreator::paintHeadline( const QString &label, const EnumerationNumber& number )
{
    painter()->save();

    QFont font = painter()->font();
    font.setBold( true );
    font.setPointSizeF( 15 pt );
    font.setFamily( "lucida" );
    painter()->setFont( font );

    QString text = label;
    if (number.isValid())
    {
        text.prepend( " " );
        text.prepend( number.toString( config["EnumerationPattern"].toString() ) );
    }
    painter()->drawText( QPointF(leftMargin(), topMargin()), text);
    m_additionalTopMargin += 2 * painter()->fontMetrics().height();
    painter()->restore();
}

void PDFCreator::paintTableOfContents()
{
    int currentPage = m_pdfPainter->currentPageNumber();
    m_tableOfContentsNumPages = 1;                     // length of toc;

    const double dotsSpace = 1;
    const double lineHeight = painter()->fontMetrics().height();

    m_pdfPainter->insertEmptyPage( m_tableOfContentsPage );
    m_pdfPainter->activatePage( m_tableOfContentsPage );

    paintHeadline( "Table of Content" );

    double y = topMargin();
    while (true)
    {
        painter()->save();
        if ( m_tableOfContents.isEmpty() ) // no more content to draw
        {
            painter()->restore();
            break;
        }
        else                              // draw more content
        {
            if ( y < pageRect().height() - bottomMargin() - lineHeight ) // content fits on page
            {
                TocEntry currentEntry = m_tableOfContents.take();
                QString prologue = currentEntry.label;
                QString epilogue = QString("%1").arg( currentEntry.pageNumber );
                int prologueWidth = painter()->fontMetrics().width( prologue );
                int epilogueWidth = painter()->fontMetrics().width( epilogue );
                double indentation = 0;
                if (currentEntry.level == 1)
                {
                    y += 1;
                    QFont font = painter()->font();
                    font.setBold( true );
                    font.setFamily( "lucida ");
                    painter()->setFont( font );
                }
                else
                {
                    indentation = 3;
                    int dotsLength = pageRect().width() - prologueWidth - epilogueWidth - leftMargin() - rightMargin() - 2 * dotsSpace - indentation;
                    QString dots = "";
                    while (dotsLength - painter()->fontMetrics().width(dots) > painter()->fontMetrics().width("."))
                    {
                        dots.append(".");
                    }
                    painter()->drawText( QPointF( indentation + leftMargin() + prologueWidth + dotsSpace, y), dots );
                }
                painter()->drawText( QPointF( indentation + leftMargin(), y), prologue  );
                painter()->drawText( QPointF( pageRect().width() - epilogueWidth - rightMargin(), y ), epilogue );

                y += lineHeight;
            }
            else        // content does not fit on page
            {
                m_tableOfContentsPage++;
                m_tableOfContentsNumPages++;
                painter()->restore();
                m_pdfPainter->insertEmptyPage( m_tableOfContentsPage );
                painter()->save();
                m_additionalTopMargin = 0; // must be set explicitly since next() is not called but insertPage.
                y = topMargin();
                // content will fit on page next iteration
            }
        }
        painter()->restore();
    }

    m_pdfPainter->activatePage( currentPage + m_tableOfContentsNumPages );
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



void PDFCreator::paintSong(const Song *song)
{
    nextPage();
    QString headline = labelSong( song );
    m_tableOfContents.append( headline, 1, m_pdfPainter->currentPageNumber() + 1 );
    paintHeadline( headline );


    INIT_LOOP;
    for (const Attachment* attachment : song->attachments())
    {
        paintAttachment( attachment );
        IFN_FIRST_ITERATION nextPage();
    }
}

bool printAttachment( const Attachment* attachment )
{
    Q_UNUSED( attachment );
    //TODO filter tags, etc

    return true;
}

void PDFCreator::paintAttachment(const Attachment *attachment )
{
    if (printAttachment( attachment ))
    {
//        m_tableOfContents.append( labelAttachment(attachment), 2, m_pdfPainter->currentPageNumber() + 1 );

        if (attachment->type() == PDFAttachment::TYPE)
        {
            paintPDFAttachment( qobject_assert_cast<const PDFAttachment*>(attachment) );
        }
        else if (attachment->type() == ChordPatternAttachment::TYPE)
        {
            paintChordPatternAttachment( qobject_assert_cast<const ChordPatternAttachment*>(attachment) );
        }
    }
}

void PDFCreator::decoratePageNumbers()
{
    double height = painter()->fontMetrics().height();
    double y = pageRect().height() - 10;
    for (int i = 0; i < m_pdfPainter->numPages(); ++i)
    {
        int pageNumber = i - m_tableOfContentsNumPages;
        if ( pageNumber > 0 )
        {
            m_pdfPainter->activatePage( i );
            QTextOption option;
            option.setAlignment( Qt::AlignCenter );
            painter()->drawText( QRectF( 0, y - height/2, pageRect().width(), height ), QString("%1").arg( pageNumber ), option );
        }
    }
}

void PDFCreator::paintPDFAttachment(const PDFAttachment* attachment )
{
    Poppler::Document* doc = attachment->document();
    if (doc)
    {
        for (int i = 0; i < doc->numPages(); ++i)
        {
            attachment->document()->page(i)->renderToPainter( painter() );
        }
    }
}


void PDFCreator::paintChordPatternAttachment( const ChordPatternAttachment* attachment )
{
    QStringList lines = attachment->chordPattern().split("\n", QString::KeepEmptyParts);

    typedef struct Paragraph
    {
        QStringList lines;
        double height;
    } Paragraph;

    QList<Paragraph> paragraphs;

    Paragraph currentParagraph;
    for (const QString& line : lines)
    {
        currentParagraph.lines << line;
        if (line.isEmpty())
        {
            currentParagraph.height = painter()->fontMetrics().height() * currentParagraph.lines.length();
            paragraphs << currentParagraph;
            currentParagraph.lines.clear();
        }
    }
    paragraphs << currentParagraph;


    double y = topMargin();
    double width = pageRect().width() - leftMargin() - rightMargin();
    double height = painter()->fontMetrics().height();
    for (const Paragraph& par : paragraphs)
    {
        double leftSpace = pageRect().height() - bottomMargin() - y;
        if (leftSpace > par.height)
        {
            for (const QString & line : par.lines )
            {
                painter()->drawText( QRectF( leftMargin(), y, width, height ), line );
                y += height;
            }
        }
        else
        {
            painter()->fillRect( QRectF( leftMargin(), y, width, par.height ), Qt::red);
            nextPage();
            y = topMargin();
        }
    }

}


