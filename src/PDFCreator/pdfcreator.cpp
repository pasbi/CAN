#include "pdfcreator.h"
#include <QPainter>
#include "Database/SongDatabase/songdatabase.h"
#include "Database/EventDatabase/event.h"
#include "pdfpagesizewrapper.h"

#define pt / m_pdfPainter->scale()

DEFN_CONFIG( PDFCreator, "PDF Export" );

CONFIGURABLE_ADD_ITEM( PDFCreator,
                       PDFTitlePattern,
                       QT_TRANSLATE_NOOP("ConfigurableItem", "Title"),
                       "Setlist\n\n-- {EventTitle} --\n\n{Begin}",
                       ConfigurationItemOptions::TextEditOptions( QT_TRANSLATE_NOOP("ConfigurableItem", "Pattern") )
                     );
CONFIGURABLE_ADD_ITEM( PDFCreator,
                       SongTitle,
                       QT_TRANSLATE_NOOP("ConfigurableItem", "Song Title"),
                       true,
                       ConfigurationItemOptions::CheckboxOptions()
                     );
CONFIGURABLE_ADD_ITEM( PDFCreator,
                       SongTitlePattern,
                       QT_TRANSLATE_NOOP("ConfigurableItem", "Song Title"),
                       "{Title} -- {Artist}",
                       ConfigurationItemOptions::LineEditOptions( QT_TRANSLATE_NOOP("ConfigurableItem", "Pattern") )
                     );
CONFIGURABLE_ADD_ITEM( PDFCreator,
                       LineSpacing,
                       QT_TRANSLATE_NOOP("ConfigurableItem", "Line spacing"),
                       1,
                       ConfigurationItemOptions::DoubleSpinBoxOptions( 0, 10, 0.01, "" )
                     );
CONFIGURABLE_ADD_ITEM( PDFCreator,
                       ChordLineSpacing,
                       QT_TRANSLATE_NOOP("ConfigurableItem", "Chord Line spacing"),
                       2,
                       ConfigurationItemOptions::DoubleSpinBoxOptions( 0, 10, 0.01, "" )
                     );
CONFIGURABLE_ADD_ITEM( PDFCreator,
                       PDFSize,
                       QT_TRANSLATE_NOOP("ConfigurableItem", "Page size"),
                       0, //TODO WRONG
                       //pageSizeToInt( QPdfWriter::A4 ),
                       ConfigurationItemOptions::ComboBoxOptions( QStringList() <<  QT_TRANSLATE_NOOP("ConfigurableItem", "A0")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "A1")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "A2")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "A3")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "A4")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "A5")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "A6")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "A7")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "A8")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "A9")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "B0")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "B1")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "B2")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "B3")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "B4")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "B5")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "B6")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "B7")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "B8")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "B9")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "B10")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "C5E")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "Comm10E")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "DLE")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "Executive")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "Folio")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "Ledger")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "Legal")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "Letter")
                                                                                <<  QT_TRANSLATE_NOOP("ConfigurableItem", "Tabloid")
                                                                    )
                     );
CONFIGURABLE_ADD_ITEM( PDFCreator,
                       AlignSongs,
                       QT_TRANSLATE_NOOP("ConfigurableItem", "Align songs: "),
                       0,
                       ConfigurationItemOptions::ComboBoxOptions( QStringList() << QT_TRANSLATE_NOOP("ConfigurableItem", "No alignment")
                                                                                << QT_TRANSLATE_NOOP("ConfigurableItem", "Odd pages")
                                                                                << QT_TRANSLATE_NOOP("ConfigurableItem", "Even pages")
                                                                                << QT_TRANSLATE_NOOP("ConfigurableItem", "Duplex")
                                                                                << QT_TRANSLATE_NOOP("ConfigurableItem", "Endless")         )
                       );
CONFIGURABLE_ADD_ITEM( PDFCreator,
                       TableOfContents,
                       QT_TRANSLATE_NOOP("ConfigurableItem", "Table of Contents"),
                       true,
                       ConfigurationItemOptions::CheckboxOptions()
                     );

CONFIGURABLE_ADD_ITEM( PDFCreator,
                       PageNumbers,
                       QT_TRANSLATE_NOOP("ConfigurableItem", "Page Numbers"),
                       true,
                       ConfigurationItemOptions::CheckboxOptions()
                     );
CONFIGURABLE_ADD_ITEM( PDFCreator,
                       PDFResolution,
                       QT_TRANSLATE_NOOP("ConfigurableItem", "Resolution"),
                       72,
                       ConfigurationItemOptions::AdvancedDoubleSliderOptions(0, 400, 1, QT_TRANSLATE_NOOP("ConfigurableItem", "dpi") )
                     );
CONFIGURABLE_ADD_ITEM( PDFCreator,
                       ContiuneOnNextPageMark,
                       QT_TRANSLATE_NOOP("ConfigurableItem", "Show to-be-continued-hint"),
                       true,
                       ConfigurationItemOptions::CheckboxOptions()
                     );

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
    title.replace( "{EventTitle}", setlist->event()->label());
    title.replace( "{Begin}", QLocale().toString( setlist->event()->beginning(), QLocale().dateTimeFormat( QLocale::ShortFormat ) ) );
    title.replace( "{End}",   QLocale().toString( setlist->event()->ending(),    QLocale().dateTimeFormat( QLocale::ShortFormat ) ) );

    for (const QString & key : DICTIONARY.keys())
    {
        title.replace(key, DICTIONARY[key]);
    }
    return title;
}

PDFCreator::PDFCreator( const Setlist *setlist, const QString& filename) :
    m_setlist( setlist ),
    m_filename( filename )
{
    QString title = labelSetlist( m_setlist );
    QPdfWriter::PageSize pageSize = QPdfWriter::A0; //intToPageSize( config["PDFSize"].toInt() ); //TODO WRONG
    m_pdfPainter = new PDFPaintDevice( title, pageSize );
    m_pdfPainter->setEndlessPageSize( isEndlessPage() );

    QFont font( "times" );
    font.setPointSizeF( 12 pt );
    m_pdfPainter->setDefaultFont( font );
}

void PDFCreator::run()
{
    paintSetlist();

    if (isInterruptionRequested())
    {
        return;
    }

    emit progress( maximum() - 3 );
    if ( config["TableOfContents"].toBool() )
    {
        emit currentTask( tr("Generate table of contents ...") );
        paintTableOfContents( );
    }


    if (isInterruptionRequested())
    {
        return;
    }
    emit progress( maximum() - 2 );
    emit currentTask( tr("Align songs ...") );
    switch (config["AlignSongs"].toInt())
    {
    case 0:
        // do not align songs;
        break;
    case 1: // odd pages
        alignSongs( 0 );
        break;
    case 2: // even pages
        alignSongs( 1 );
        break;
    case 3: // duplex
        optimizeForDuplex();
        break;
    case 4: // endless
        // no actions needed
        break;
    }

    if (isInterruptionRequested())
    {
        return;
    }
    emit progress( maximum() - 1);
    emit currentTask( tr("Generate page numbers ...") );
    if ( config["PageNumbers"].toBool() )
    {
        decoratePageNumbers();
    }

    if (isInterruptionRequested())
    {
        return;
    }
    emit progress( maximum() );
    emit currentTask( tr("Save pdf ...") );
    save( m_filename );
}

int PDFCreator::maximum() const
{
    return m_setlist->items().length() + 4; // toc, align, pagenumbers, save
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

PicturePainter* PDFCreator::painter()
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

    int i = 0;
    for ( const Song* song : m_setlist->songs() )
    {
        if (!paintSong( song ))
        {
            // interruption has been requested
            return;
        }
        emit progress(i++);
    }
}

void PDFCreator::paintHeadline(const QString &label)
{
    painter()->QPainter::save();

    QFont font = painter()->font();
    font.setBold( true );
    font.setPointSizeF( 15 pt );
    font.setFamily( "lucida" );
    painter()->setFont( font );

    QString text = label;

    painter()->drawText( QPointF(leftMargin(), topMargin()), text);
    m_additionalTopMargin += 1.2 * painter()->fontMetrics().height();
    painter()->restore();
}

void PDFCreator::paintTableOfContents()
{
    typedef struct PageNumberStub {
        PageNumberStub( int page, int y ) :
            page( page ),
            y( y )
        {}
        const int page;
        const int y;
    } PageNumberStub;


    m_pdfPainter->insertEmptyPage( m_tableOfContentsPage, PicturePainter::TableOfContentsStartsHere );
    m_tableOfContentsNumPages = 1;                     // length of toc;

    paintHeadline( QObject::tr("Table of Content") );

    // draw entries and find places to fill in page numbers
    QList<PageNumberStub> pageNumberStubs;
    const double lineHeight = painter()->fontMetrics().height();
    double y = topMargin();

    painter()->QPainter::save();
    QFont font = painter()->font();
    font.setBold( true );
    font.setFamily( "lucida" );
    painter()->setFont( font );
    while (!m_tableOfContents.isEmpty())
    {
        if ( y < pageRect().height() - bottomMargin() - lineHeight ) // content fits on page
        {
            QString currentEntry = m_tableOfContents.takeFirst();
            painter()->drawText( QPointF( leftMargin(), y), currentEntry  );
            pageNumberStubs << PageNumberStub( m_pdfPainter->currentPageNumber(), y );

            y += lineHeight;
        }
        else        // content does not fit on page
        {
            if (isEndlessPage())
            {
                //?
            }
            else
            {
                m_tableOfContentsPage++;
                m_tableOfContentsNumPages++;
                m_pdfPainter->insertEmptyPage( m_tableOfContentsPage, PicturePainter::NothingSpecial );
                m_additionalTopMargin = 0; // must be set explicitly since next() is not called but insertPage.
                y = topMargin();
                // content will fit on page next iteration
            }
        }
    }


    // replace page number stubs

    int pageNumber = -1;
    for (const PageNumberStub& stub : pageNumberStubs)
    {
        // find next pagenumber
        for (int i = pageNumber + 1; i < m_pdfPainter->numPages(); ++i)
        {
            m_pdfPainter->activatePage( i );
            if (m_pdfPainter->currentPage()->flag() == PicturePainter::SongStartsHere)
            {
                pageNumber = i;
                break;
            }
        }

        QString text = QString("%1").arg(pageNumber + 1);
        m_pdfPainter->activatePage( stub.page );
        double width = painter()->fontMetrics().boundingRect( text ).width();
        painter()->drawText( QPointF( m_pdfPainter->size().width() - rightMargin() - width, stub.y), text );

    }
    painter()->restore();
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



bool PDFCreator::paintSong(const Song *song)
{
    nextPage( PicturePainter::SongStartsHere );
    QString headline = labelSong( song );
    m_tableOfContents.append( headline );
    paintHeadline( headline );

    int i = 0;
    for ( Attachment* attachment : song->attachments() )
    {
        if (isInterruptionRequested()) return false;
        emit currentTask( QString(tr("Draw attachment %1 of song %2")).arg(song->title()).arg(attachment->name()) );

        if (i++ != 0)
        {
            nextPage( PicturePainter::NothingSpecial );
        }
        paintAttachment( attachment );
    }
    return true;
}

//bool printAttachment( const Attachment* attachment )
//{
//    Q_UNUSED( attachment );
//    //TODO filter tags, etc

//    return true;
//}

void PDFCreator::paintAttachment( Attachment *attachment )
{
    if (/*printAttachment( attachment )*/ true)
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
        m_pdfPainter->activatePage( i );
        QTextOption option;
        option.setAlignment( Qt::AlignCenter );
        painter()->drawText( QRectF( 0, y - height/2, pageRect().width(), height ), QString("%1").arg( i + 1 ), option );
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
            painter()->QPainter::save();
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

bool pageBreakDEP( const QStringList & lines, const int currentLine, const double heightLeft, const QPainter& painter )
{
    // return whether we must use another page to fit the content
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
    qDebug() << "paint " << attachment << "on page" << currentPage();
    painter()->QPainter::save();
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

        if (isEndlessPage())
        {
            double height = painter()->pageSize().size(QPageSize::Point).height() / 6.5;    // why 6.5?
            double spaceLeft = height - y;

            if (spaceLeft < 0)
            {
                currentPage()->growDown( -spaceLeft );
            }
        }
        else
        {
            if ( pageBreakDEP( lines,
                            i,
                            pageRect().height() - bottomMargin() - y,
                            *painter()                                   ))
            {
                    if (config["ContiuneOnNextPageMark"].toBool())
                    {
                        drawContinueOnNextPageMark();
                    }

                    painter()->restore();
                    nextPage( PicturePainter::NothingSpecial );
                    painter()->QPainter::save();
                    configurePainter( painter() );
                    y = topMargin();
            }
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
            painter()->QPainter::save();

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

void PDFCreator::optimizeForDuplex()
{
    // an odd and the following even page will be printed on the same piece of paper.
    // So ensure that no song is on more than one side of the same paper.
    bool songsStarted = false;
    for (int currentPageNum = 0; currentPageNum < m_pdfPainter->numPages(); ++currentPageNum)
    {
        m_pdfPainter->activatePage( currentPageNum );
        if ( songsStarted
             && currentPage()->flag() != PicturePainter::SongStartsHere   // start may be everywhere, but song shall only be continued on odd pages
             && currentPageNum % 2 != 0 )
        {
            m_pdfPainter->insertEmptyPage( currentPageNum, PicturePainter::NothingSpecial );
        }
        else if (currentPage()->flag() == PicturePainter::SongStartsHere)
        {
            songsStarted = true;
        }
    }
}

void PDFCreator::drawContinueOnNextPageMark()
{
    painter()->QPainter::save();
    QFont font = painter()->font();
    font.setBold( true );
    font.setPointSizeF( font.pointSizeF() * 3 );
    painter()->setFont( font );
    QTextOption option;
    option.setAlignment( Qt::AlignBottom | Qt::AlignRight );
    painter()->drawText( pageRect(), QChar(0x293E), option );
    painter()->restore();
}

bool PDFCreator::isEndlessPage() const
{
    return config["AlignSongs"].toInt() == 4;
}

