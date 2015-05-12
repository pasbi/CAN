#include "pdfcreator.h"
#include <QPainter>
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
                       LineSpacing,
                       "Line spacing",
                       1,
                       ConfigurationItemOptions::DoubleSpinBoxOptions( 0, 10, 0.01, "" ) );
CONFIGURABLE_ADD_ITEM( PDFCreator,
                       ChordLineSpacing,
                       "Chord Line spacing",
                       2,
                       ConfigurationItemOptions::DoubleSpinBoxOptions( 0, 10, 0.01, "" ) );
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
                       AlignSongs,
                       "Align songs: ",
                       0,
                       ConfigurationItemOptions::ComboBoxOptions( QStringList() << "No alignment"
                                                                                << "Odd pages"
                                                                                << "Even pages" ) );
CONFIGURABLE_ADD_ITEM( PDFCreator,
                       TableOfContents,
                       "Table of Contents",
                       true,
                       ConfigurationItemOptions::CheckboxOptions() );

CONFIGURABLE_ADD_ITEM( PDFCreator,
                       PageNumbers,
                       "Page Numbers",
                       true,
                       ConfigurationItemOptions::CheckboxOptions() );

CONFIGURABLE_ADD_ITEM( PDFCreator,
                       PDFResolution,
                       "Resolution",
                       72,
                       ConfigurationItemOptions::AdvancedDoubleSliderOptions(0, 400, 1, "dpi") );


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

    if ( config["TableOfContents"].toBool() )
    {
        paintTableOfContents( );
    }

    switch (config["AlignSongs"].toInt())
    {
    case 0:
        // do not align songs;
        break;
    case 1:
        alignSongs( 0 );
        break;
    case 2:
        alignSongs( 1 );
        break;
    }

    if ( config["PageNumbers"].toBool() )
    {
        decoratePageNumbers();
    }
}



PDFCreator::~PDFCreator()
{
    delete m_pdfPainter;
    m_pdfPainter = NULL;
}

void PDFCreator::nextPage(PicturePainter::Flag flag )
{
    m_additionalTopMargin = 0;
    assert( m_pdfPainter );
    m_pdfPainter->nextPage( flag );
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
    nextPage( PicturePainter::NothingSpecial );
    paintTitle();
    insertTableOfContentsStub();

    for ( const Song* song : m_setlist->songs() )
    {
        paintSong( song );
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

    m_pdfPainter->insertEmptyPage( m_tableOfContentsPage, PicturePainter::TableOfContentsStartsHere );

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
                m_pdfPainter->insertEmptyPage( m_tableOfContentsPage, PicturePainter::NothingSpecial );
                painter()->save();
                m_additionalTopMargin = 0; // must be set explicitly since next() is not called but insertPage.
                y = topMargin();
                // content will fit on page next iteration
            }
        }
        painter()->restore();
    }

//    m_pdfPainter->activatePage( currentPage + m_tableOfContentsNumPages ); //TODO required?
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
    nextPage( PicturePainter::SongStartsHere );
    QString headline = labelSong( song );
    m_tableOfContents.append( headline, 1, m_pdfPainter->currentPageNumber() + 1 );
    paintHeadline( headline );


    int i = 0;
    for ( Attachment* attachment : song->attachments() )
    {
        if (i++ != 0)
        {
            nextPage( PicturePainter::NothingSpecial );
        }
        paintAttachment( attachment );
    }
}

bool printAttachment( const Attachment* attachment )
{
    Q_UNUSED( attachment );
    //TODO filter tags, etc

    return true;
}

void PDFCreator::paintAttachment( Attachment *attachment )
{
    if (printAttachment( attachment ))
    {
        if (attachment->type() == PDFAttachment::TYPE)
        {
            paintPDFAttachment( qobject_assert_cast<PDFAttachment*>(attachment) );
        }
        else if (attachment->type() == ChordPatternAttachment::TYPE)
        {
            paintChordPatternAttachment( qobject_assert_cast<ChordPatternAttachment*>(attachment) );
        }
    }
}

void PDFCreator::decoratePageNumbers()
{
    double height = painter()->fontMetrics().height();
    double y = pageRect().height() - bottomMargin() + painter()->fontMetrics().height();
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

#include <QLabel>
#include <QDialog>
#include <QVBoxLayout>
void PDFCreator::paintPDFAttachment( PDFAttachment* attachment )
{
    // ensure that there is the right loaded
    attachment->open();
    Poppler::Document* doc = attachment->document();
    if (doc)
    {
        Poppler::Document::RenderBackend backendBefore = doc->renderBackend();
        doc->setRenderBackend( Poppler::Document::ArthurBackend );  // the other backend will not work.
        for (int i = 0; i < doc->numPages(); ++i)
        {
            if (i != 0)
            {
                nextPage( PicturePainter::NothingSpecial );
            }
            painter()->save();
            QSizeF pageSize = doc->page(i)->pageSizeF();
            QSizeF targetSize = pageRect().size();
            double fx = targetSize.width() / pageSize.width();
            double fy = targetSize.height() / pageSize.height();

            double resolution = config["PDFResolution"].toDouble();
            double f = qMin(fx, fy) * 72.0 / resolution;

            painter()->scale( f, f );

            doc->page(i)->renderToPainter( painter(), resolution, resolution );
            painter()->restore();
        }
        doc->setRenderBackend( backendBefore );
    }
}

bool pageBreak( const QStringList & lines, const int currentLine, const double heightLeft, const QPainter& painter )
{
    if (lines[currentLine].isEmpty())
    {
        // we are currently at a paragraph break
        // if next paragraph fits, return false, true otherwise.
        double paragraphHeight = 0;
        // sum line heights until the next empty line.
        for (int i = currentLine + 1; i < lines.length() && !lines[i].isEmpty(); i++ )
        {
            paragraphHeight += painter.fontMetrics().height();
        }

        return paragraphHeight > heightLeft;
    }
    else
    {
        // we are not at a paragraph break. break if the current line will not fit anymore.
        return painter.fontMetrics().height() > heightLeft;
    }
}

void configurePainter( QPainter* painter )
{
    QFont font = painter->font();
    font.setFamily( "Courier" );
    painter->setFont( font );
}

void PDFCreator::paintChordPatternAttachment( const ChordPatternAttachment* attachment )
{
    painter()->save();
    configurePainter( painter() );
    QStringList lines = attachment->chordPattern().split("\n", QString::KeepEmptyParts);


    double y = topMargin();
    double height = painter()->fontMetrics().height();
    for (int i = 0; i < lines.length(); ++i)
    {
        QString line = lines[i];
        QStringList unusedA, unusedB;
        bool isChordLine = Chord::parseLine( line , unusedA, unusedB );
        if (i != 0)
        {
            if (isChordLine)
            {
                y += height * config["ChordLineSpacing"].toDouble();
            }
            else
            {
                y += height * config["LineSpacing"].toDouble();
            }
        }

        if ( pageBreak( lines,
                        i,
                        pageRect().height() - bottomMargin() - y,
                        *painter()                                   ))
        {
            painter()->restore();
            nextPage( PicturePainter::NothingSpecial );
            painter()->save();
            configurePainter( painter() );
            y = topMargin();
        }

        QRegExp regexp( Chord::SPLIT_PATTERN );
        QStringList tokens;
        int lastJ = 0;
        int j = 0;
        while ( (j = regexp.indexIn(line, j)) >= 0 )
        {
            int n;
            n = regexp.matchedLength();
            tokens << line.mid(lastJ, j - lastJ);
            tokens << line.mid(j, n);
            lastJ = j + 1;
            j += n;
        }
        tokens << line.mid(lastJ);

        int pos = leftMargin();
        for (const QString & token : tokens)
        {
            painter()->save();

            if (isChordLine && Chord(token).isValid())
            {
                QFont font = painter()->font();
                font.setBold( true );
                painter()->setFont( font );
            }
            int w = painter()->fontMetrics().width( token );
            painter()->drawText( QRectF( pos, y, pos + w, height ), token );
            pos += w;

            painter()->restore();
        }
    }
    painter()->restore();
}

PicturePainter* PDFCreator::currentPage() const
{
    return m_pdfPainter->currentPage();
}

int lengthOfSong( int start, PDFPaintDevice* pdfPainter )
{
    int currentPage = pdfPainter->currentPageNumber();

    int length = pdfPainter->numPages() - start;
    for (int i = start + 1; i < pdfPainter->numPages(); ++i)
    {
        pdfPainter->activatePage( i );
        if (pdfPainter->currentPage()->flag() == PicturePainter::SongStartsHere)
        {
            length = i - start;
            break;
        }
    }
    pdfPainter->activatePage( currentPage );
    return length;
}

void PDFCreator::alignSongs(int remainder)
{
    for (int currentPageNum = 0; currentPageNum < m_pdfPainter->numPages(); ++currentPageNum)
    {
        m_pdfPainter->activatePage( currentPageNum );

        if (    currentPage()->flag() == PicturePainter::SongStartsHere            // if song starts here
             && currentPageNum % 2 != remainder                                    // if song starts on wrong page
             && lengthOfSong( currentPageNum, m_pdfPainter ) % 2 == 0 )            // if song length is even. Else, optimizing beginning
                                                                                   // will destroy end-optimum
        {
            m_pdfPainter->insertEmptyPage( currentPageNum, PicturePainter::NothingSpecial );
            currentPageNum++;
        }
    }
}


