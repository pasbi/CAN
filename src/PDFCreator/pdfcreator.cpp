#include "pdfcreator.h"
#include <QPdfWriter>
#include "global.h"
#include "Database/EventDatabase/event.h"
#include "Database/SongDatabase/songdatabase.h"

DEFN_CONFIG( PDFCreator, "PDF Export" );

#include "pdfcreatorconfig.h"

PDFCreator::PDFCreator(QSizeF baseSizeMM, Setlist* setlist, const QString &filename) :
    m_baseSizeMM( baseSizeMM ),
    m_setlist( setlist ),
    m_filename( filename )
{

}

PDFCreator::~PDFCreator()
{
    qDeleteAll( m_pages );
    m_pages.clear();
}

void PDFCreator::save(const QString &filename)
{
    QPdfWriter writer( filename );

    writer.setMargins( {0, 0, 0, 0} );
    QPageSize pageSize( m_baseSizeMM, QPageSize::Millimeter );
    writer.setPageSize( pageSize );
    writer.setTitle( QFileInfo(filename).baseName() );
    QPainter painter( &writer );

    bool isEmpty = true;
    for (int i = 0; i < m_pages.size(); ++i)
    {
        writer.setPageSizeMM( m_pages[i]->sizeInMM() );
        if (i != 0)
        {
            assert( writer.newPage() );
        }


        painter.drawPicture( QPoint(0, 0), m_pages[i]->picture() );
        isEmpty = false;
    }

    if (isEmpty)
    {
        qWarning() << "Empty PDF will not be created.";
    }
}

QPainter& PDFCreator::currentPainter()
{
    return currentPage()->painter();
}

QSizeF PDFCreator::currentSizeInMM() const
{
    return currentPage()->sizeInMM();
}

QSizeF PDFCreator::currentSizePainter() const
{
    return currentPage()->sizePainter();
}

void PDFCreator::newPage( Page::Flags flags, int i )
{
    if (i < 0)
    {
        i = m_pages.length();
    }

    m_additionalTopMargin = 0;
    m_pages.insert( i, new Page( m_baseSizeMM, flags ) );
    activatePage( i );
}

void PDFCreator::activatePage( int i )
{
    assert( i >= 0 && i < m_pages.length() );
    m_currentIndex = i;
}

int PDFCreator::currentIndex() const
{
    return m_currentIndex;
}

Page* PDFCreator::currentPage()
{
    assert( m_currentIndex >= 0 && m_currentIndex < m_pages.length() );
    return m_pages[ m_currentIndex ];
}

const Page* PDFCreator::currentPage() const
{
    assert( m_currentIndex >= 0 && m_currentIndex < m_pages.length() );
    return m_pages[ m_currentIndex ];
}

void PDFCreator::notifyCurrentTaskChanged( const QString &message)
{
    emit progress( m_currentStep++ );
    emit currentTaskChanged( message + " ..." );
}

void PDFCreator::run()
{
    m_currentStep = 0;
    // draw the songs, mark table of contents position and gather
    // table of contents entries.
    paintSetlist();

    if (isInterruptionRequested())
    {
        return;
    }

    notifyCurrentTaskChanged( tr("Table of contents") );
    if (config["TableOfContents"].toBool())
    {
        paintTableOfContents();
    }

    if (isInterruptionRequested())
    {
        return;
    }

    notifyCurrentTaskChanged( tr("Align songs") );
    switch (config["AlignSongs"].toInt())
    {
    case 0: // do not align songs
        break;
    case 1: // oddPages
        alignSongs( 0 );
        break;
    case 2: // evenPages
        alignSongs( 1 );
        break;
    case 3: // duplex
        optimizeForDuplex();    //TODO distinguish duplex even/odd
        break;
    case 4: // endless, songs will not be alignt either.
        break;
    }

    if (isInterruptionRequested())
    {
        return;
    }

    notifyCurrentTaskChanged( tr("Generate page numbers") );
    if (config["PageNumbers"].toBool())
    {
        decoratePageNumbers();
    }

    if (isInterruptionRequested())
    {
        return;
    }

    notifyCurrentTaskChanged( tr("Save PDF") );
    save( m_filename );
}

QRectF PDFCreator::pageRect() const
{
    return QRectF( QPointF(), currentSizePainter() );
}

QRectF PDFCreator::pageRectMargins() const
{
    return QRectF( QPointF( leftMargin(),
                            topMargin() ),
                   QPointF( currentSizePainter().width()  - leftMargin() - rightMargin(),
                            currentSizePainter().height() - topMargin() -  bottomMargin() ) );
}

bool PDFCreator::isEndlessPage() const
{
    return config["AlignSongs"].toInt() == 4;
}


//////////////////////////////////////////////
////
///  pdf paint member
//
/////////////////////////////////////////////

QMap<QString, QString> PDFCreator::dictionary()
{
    // define nice replacements, e.g. replace -- with a real hyphen.
    return QMap<QString, QString>(
    {
    { "--", QChar(0x2014) }
    });
}

QString labelSetlist(Setlist *setlist)
{
    QString title = PDFCreator::config["PDFTitlePattern"].toString();
    title.replace( "{EventTitle}", setlist->event()->label());
    title.replace( "{Begin}", QLocale().toString( setlist->event()->beginning(), QLocale().dateTimeFormat( QLocale::ShortFormat ) ) );
    title.replace( "{End}",   QLocale().toString( setlist->event()->ending(),    QLocale().dateTimeFormat( QLocale::ShortFormat ) ) );


    for (const QString & key : PDFCreator::dictionary().keys())
    {
        title.replace(key, PDFCreator::dictionary()[key]);
    }
    return title;
}

void PDFCreator::paintHeadline(const QString& label)
{
    currentPainter().save();

    QFont font = currentPainter().font();
    font.setBold( true );
    font.setPointSizeF( 15 );
    font.setFamily( "lucida" );
    currentPainter().setFont( font );

    qDebug() << topMargin();
    double fontHeight = currentPainter().fontMetrics().height();
    currentPainter().drawText( QPointF(leftMargin(), topMargin() + fontHeight), label);
    m_additionalTopMargin += 2 * fontHeight;
    currentPainter().restore();
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
    for (const QString & key : PDFCreator::dictionary().keys())
    {
        pattern.replace(key, PDFCreator::dictionary()[key]);
    }
    return pattern;
}

void PDFCreator::paintTitle()
{
    QString title = labelSetlist( m_setlist );
    currentPainter().drawText( pageRectMargins(), Qt::AlignCenter, title );
}

void PDFCreator::insertTableOfContentsStub()
{
    m_tableOfContentsPage = currentIndex() + 1;
}

bool PDFCreator::paintSong(const Song* song)
{
    newPage( Page::SongStartsHere );
    QString headline = labelSong( song );
    m_tableOfContents.append( headline );
    paintHeadline( headline );

    int i = 0;
    for ( Attachment* attachment : song->attachments() )
    {
        if (isInterruptionRequested()) return false;
        notifyCurrentTaskChanged( QString(tr("Draw attachment %1 of song %2"))
                                      .arg(song->title())
                                      .arg(attachment->name())                  );

        if (i++ != 0)
        {
            newPage( Page::NothingSpecial );
        }
        paintAttachment( attachment );
    }
    return true;
}

bool printAttachment( const Attachment* attachment )
{
    Q_UNUSED( attachment );
    //TODO filter tags, etc

    return true;
}

void PDFCreator::paintAttachment(Attachment *attachment)
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
                newPage( Page::NothingSpecial );
            }
            currentPainter().save();
            QSizeF pageSize = doc->page(i)->pageSizeF();
            QSizeF targetSize = pageRect().size();
            double fx = targetSize.width() / pageSize.width();
            double fy = targetSize.height() / pageSize.height();

            double resolution = config["PDFResolution"].toDouble();
            double f = qMin(fx, fy) * 72.0 / resolution;

            currentPainter().scale( f, f );

            doc->page(i)->renderToPainter( &currentPainter(), resolution, resolution );
            currentPainter().restore();
        }
        doc->setRenderBackend( backendBefore );
    }
}

void configurePainter( QPainter& painter )
{
    QFont font = painter.font();
    font.setFamily( "Courier" );
    painter.setFont( font );
}

bool pageBreak( const QStringList & lines, const int currentLine, const double heightLeft, const QPainter& painter )
{
    // return whether we must use another page to fit the content
    if (lines[currentLine].isEmpty())
    {
        // we are currently at a paragraph break
        // if next paragraph fits, return false, true otherwise.
        double paragraphHeight = 0;
        double lineHeight =  painter.fontMetrics().height();
        // sum line heights until the next empty line.

        QString paragraph;
        for (int i = currentLine; i < lines.length(); i++ )
        {
            if (i != currentLine && lines[i].isEmpty())
            {
                // paragraph ends
                break;
            }
            QStringList unusedA, unusedB;
            bool isChordLine = Chord::parseLine( lines[i] , unusedA, unusedB );
            if (i != 0)
            {
                if (isChordLine)
                {
                    paragraphHeight += lineHeight * PDFCreator::config["ChordLineSpacing"].toDouble();
                }
                else
                {
                    paragraphHeight += lineHeight * PDFCreator::config["LineSpacing"].toDouble();
                }
            }
            paragraph += lines[i] + "\n";
        }

        return paragraphHeight > heightLeft;
    }
    else
    {
        // we are not at a paragraph break. break if the current line will not fit anymore.
        return painter.fontMetrics().height() > heightLeft;
    }
}

void PDFCreator::drawContinueOnNextPageMark()
{
    currentPainter().save();
    QFont font = currentPainter().font();
    font.setBold( true );
    font.setPointSizeF( font.pointSizeF() * 3 );
    currentPainter().setFont( font );
    QTextOption option;
    option.setAlignment( Qt::AlignBottom | Qt::AlignRight );
    currentPainter().drawText( pageRectMargins(), QChar(0x293E), option );
    currentPainter().restore();
}

void PDFCreator::paintChordPatternAttachment(ChordPatternAttachment *attachment)
{
    currentPainter().save();
    configurePainter( currentPainter() );
    QStringList lines = attachment->chordPattern().split("\n", QString::KeepEmptyParts);


    double y = topMargin();
    double height = currentPainter().fontMetrics().height();
    for (int i = 0; i < lines.length(); ++i)
    {
        QString line = lines[i];

        if (isEndlessPage())
        {
            double spaceLeft = pageRectMargins().height() - y;

            if (spaceLeft < 0)
            {
                // space left must be converted from painter-units to mm

                currentPage()->growDownMM( -currentPage()->painterUnitsInMM( spaceLeft ) );
            }
        }
        else
        {
            if ( pageBreak( lines,
                            i,
                            pageRectMargins().bottom() - y,
                            currentPainter()                            ))
            {
                    if (config["ContiuneOnNextPageMark"].toBool())
                    {
                        drawContinueOnNextPageMark();
                    }


                    currentPainter().restore();
                    newPage( Page::NothingSpecial );
                    currentPainter().save();
                    configurePainter( currentPainter() );
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

        QStringList unusedA, unusedB;
        bool isChordLine = Chord::parseLine( line , unusedA, unusedB );

        int pos = leftMargin();
        for (const QString & token : tokens)
        {
            currentPainter().save();

            if (isChordLine && Chord(token).isValid())
            {
                QFont font = currentPainter().font();
                font.setBold( true );
                currentPainter().setFont( font );
            }
            int w = currentPainter().fontMetrics().width( token );
            currentPainter().drawText( QRectF( pos, y, pos + w, height ), token );
            pos += w;

            currentPainter().restore();
        }

        if (isChordLine)
        {
            y += height * config["ChordLineSpacing"].toDouble();
        }
        else
        {
            y += height * config["LineSpacing"].toDouble();
        }
    }
    currentPainter().restore();
}

void PDFCreator::paintSetlist()
{
    newPage( Page::NothingSpecial );
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

    newPage( Page::TableOfContentsStartsHere, m_tableOfContentsPage );

    paintHeadline( QObject::tr("Table of Content") );

    // draw entries and find places to fill in page numbers
    QList<PageNumberStub> pageNumberStubs;
    const double lineHeight = currentPainter().fontMetrics().height();
    double y = topMargin();

    currentPainter().save();
    QFont font = currentPainter().font();
    font.setBold( true );
    font.setFamily( "lucida" );
    currentPainter().setFont( font );
    while (!m_tableOfContents.isEmpty())
    {
        if ( y < pageRect().height() - bottomMargin() - lineHeight ) // content fits on page
        {
            QString currentEntry = m_tableOfContents.takeFirst();
            currentPainter().drawText( QPointF( leftMargin(), y), currentEntry  );
            pageNumberStubs << PageNumberStub( currentIndex(), y );

            y += lineHeight * 1.1;
        }
        else        // content does not fit on page
        {
            if (isEndlessPage())
            {
                //? //TODO
            }
            else
            {
                m_tableOfContentsPage++;
                newPage( Page::NothingSpecial, m_tableOfContentsPage );
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
        for (int i = pageNumber + 1; i < m_pages.length(); ++i)
        {
            activatePage( i );
            if ( currentPage()->flags() & Page::SongStartsHere)
            {
                pageNumber = i;
                break;
            }
        }

        QString text = QString("%1").arg( pageNumber + 1 );
        activatePage( stub.page );
        double textWidth = currentPainter().fontMetrics().boundingRect( text ).width();
        double x = pageRectMargins().right() - textWidth;
        currentPainter().drawText( QPointF( x, stub.y), text );

    }
    currentPainter().restore();
}

void PDFCreator::alignSongs( int mode )
{
    for (int currentPageNum = 0; currentPageNum < m_pages.length(); ++currentPageNum)
    {
        activatePage( currentPageNum );

        if (    (currentPage()->flags() & Page::SongStartsHere)                    // if song starts here
             && currentPageNum % 2 != mode                                         // if song starts on wrong page
             && lengthOfSong( currentPageNum ) % 2 == 0 )                          // if song length is even. Else, optimizing beginning
                                                                                   // will destroy end-optimum
        {
            newPage( Page::NothingSpecial, currentPageNum );
            currentPageNum++;
        }
    }
}

int PDFCreator::lengthOfSong( int start )
{
    int savedIndex = currentIndex();

    int length = m_pages.length() - start;
    for (int i = start + 1; i < m_pages.length(); ++i)
    {
        activatePage( i );
        if (currentPage()->flags() & Page::SongStartsHere)
        {
            length = i - start;
            break;
        }
    }

    // activate the page that was active before this function call.
    activatePage( savedIndex );
    return length;
}

void PDFCreator::optimizeForDuplex( )
{
    // an odd and the following even page will be printed on the same piece of paper.
    // So ensure that no song is on more than one side of the same paper.
    bool songsStarted = false;
    for (int currentPageNum = 0; currentPageNum < m_pages.length(); ++currentPageNum)
    {
        activatePage( currentPageNum );
        if ( songsStarted
             && !(currentPage()->flags() & Page::SongStartsHere)   // start may be everywhere, but song shall only be continued on odd pages
             && currentPageNum % 2 != 0 )
        {
            newPage( Page::NothingSpecial, currentPageNum );
        }
        else if (currentPage()->flags() & Page::SongStartsHere)
        {
            songsStarted = true;
        }
    }
}

void PDFCreator::decoratePageNumbers()
{
    double height = currentPainter().fontMetrics().height();
    for (int i = 0; i < m_pages.length(); ++i)
    {
        activatePage( i );

        double y = pageRect().height() - bottomMargin();
        currentPainter().drawText( QRectF( 0, y - height/2, pageRect().width(), height ),
                                   QString("%1").arg( i + 1 ),
                                   QTextOption( Qt::AlignCenter )                       );
    }
}
