#include "endlesspdfcreator.h"
#include <QPdfWriter>
#include "global.h"
#include "Database/EventDatabase/event.h"
#include "Database/SongDatabase/songdatabase.h"

DEFN_CONFIG( EndlessPDFCreator, "PDF Export" );

#include "pdfcreatorconfig.h"

EndlessPDFCreator::EndlessPDFCreator(QSizeF baseSizeMM, Setlist* setlist, const QString &filename) :
    m_baseSizeMM( baseSizeMM ),
    m_setlist( setlist ),
    m_filename( filename )
{

}

EndlessPDFCreator::~EndlessPDFCreator()
{
    qDeleteAll( m_pages );
    m_pages.clear();
}

void EndlessPDFCreator::save(const QString &filename)
{
    QPdfWriter writer( filename );

//    writer.setMargins( {0, 0, 0, 0} );
    QPageSize pageSize( m_baseSizeMM, QPageSize::Millimeter );
    writer.setPageSize( pageSize );
    writer.setTitle( "TEST" );
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

QPainter& EndlessPDFCreator::currentPainter()
{
    return currentPage()->painter();
}

QSizeF EndlessPDFCreator::currentSizeInMM() const
{
    return currentPage()->sizeInMM();
}

QSizeF EndlessPDFCreator::currentSizePainter() const
{
    return currentPage()->sizePainter();
}

void EndlessPDFCreator::newPage( Page::Flags flags, int i )
{
    if (i < 0)
    {
        i = m_pages.length();
    }

    m_additionalTopMargin = 0;
    m_pages.insert( i, new Page( m_baseSizeMM, flags ) );
    activatePage( i );
}

void EndlessPDFCreator::activatePage( int i )
{
    assert( i >= 0 && i < m_pages.length() );
    m_currentIndex = i;
}

int EndlessPDFCreator::currentIndex() const
{
    return m_currentIndex;
}

Page* EndlessPDFCreator::currentPage()
{
    assert( m_currentIndex >= 0 && m_currentIndex < m_pages.length() );
    return m_pages[ m_currentIndex ];
}

const Page* EndlessPDFCreator::currentPage() const
{
    assert( m_currentIndex >= 0 && m_currentIndex < m_pages.length() );
    return m_pages[ m_currentIndex ];
}

void EndlessPDFCreator::notifyCurrentTaskChanged( const QString &message)
{
    emit progress( m_currentStep++ );
    emit currentTaskChanged( message + " ..." );
}

void EndlessPDFCreator::run()
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

QRectF EndlessPDFCreator::pageRect() const
{
    return QRectF( QPointF(), currentSizePainter() );
}

QRectF EndlessPDFCreator::pageRectMargins() const
{
    return QRectF( QPointF( leftMargin(),
                            topMargin() ),
                   QPointF( currentSizePainter().width()  - leftMargin() - rightMargin(),
                            currentSizePainter().height() - topMargin() -  bottomMargin() ) );
}

bool EndlessPDFCreator::isEndlessPage() const
{
    return config["AlignSongs"].toInt() == 4;
}


//////////////////////////////////////////////
////
///  pdf paint member
//
/////////////////////////////////////////////

QMap<QString, QString> EndlessPDFCreator::dictionary()
{
    // define nice replacements, e.g. replace -- with a real hyphen.
    return QMap<QString, QString>(
    {
    { "--", QChar(0x2014) }
    });
}

QString labelSetlistDEP(Setlist *setlist)
{
    QString title = EndlessPDFCreator::config["PDFTitlePattern"].toString();
    title.replace( "{EventTitle}", setlist->event()->label());
    title.replace( "{Begin}", QLocale().toString( setlist->event()->beginning(), QLocale().dateTimeFormat( QLocale::ShortFormat ) ) );
    title.replace( "{End}",   QLocale().toString( setlist->event()->ending(),    QLocale().dateTimeFormat( QLocale::ShortFormat ) ) );


    for (const QString & key : EndlessPDFCreator::dictionary().keys())
    {
        title.replace(key, EndlessPDFCreator::dictionary()[key]);
    }
    return title;
}

void EndlessPDFCreator::paintHeadline(const QString& label)
{
    currentPainter().save();

    QFont font = currentPainter().font();
    font.setBold( true );
    font.setPointSizeF( 15 );
    font.setFamily( "lucida" );
    currentPainter().setFont( font );

    QString text = label;

    currentPainter().drawText( QPointF(leftMargin(), topMargin()), text);
    m_additionalTopMargin += 1.2 * currentPainter().fontMetrics().height();
    currentPainter().restore();
}

QString labelSongDEP( const Song* song )
{
    QString pattern = EndlessPDFCreator::config["SongTitlePattern"].toString();
    for (int i = 0; i < song->database()->attributeKeys().length(); ++i)
    {
        QString attribute = song->attribute(i).toString();
        QString key = song->database()->attributeKeys()[i];
        key = SongDatabase::extractHeaderLabel( key );
        pattern = pattern.replace( QString("{%1}").arg(key), attribute );
    }
    for (const QString & key : EndlessPDFCreator::dictionary().keys())
    {
        pattern.replace(key, EndlessPDFCreator::dictionary()[key]);
    }
    return pattern;
}

void EndlessPDFCreator::paintTitle()
{
    QString title = labelSetlistDEP( m_setlist );
    currentPainter().drawText( pageRect(), Qt::AlignCenter, title );
}

void EndlessPDFCreator::insertTableOfContentsStub()
{
    m_tableOfContentsPage = currentIndex() + 1;
}

bool EndlessPDFCreator::paintSong(const Song* song)
{
    newPage( Page::SongStartsHere );
    QString headline = labelSongDEP( song );
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

void EndlessPDFCreator::paintAttachment(Attachment *attachment)
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

void EndlessPDFCreator::paintPDFAttachment( PDFAttachment* attachment )
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
                    paragraphHeight += lineHeight * EndlessPDFCreator::config["ChordLineSpacing"].toDouble();
                }
                else
                {
                    paragraphHeight += lineHeight * EndlessPDFCreator::config["LineSpacing"].toDouble();
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

void EndlessPDFCreator::drawContinueOnNextPageMark()
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

void EndlessPDFCreator::paintChordPatternAttachment(ChordPatternAttachment *attachment)
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
            double height = currentPage()->sizePainter().height() / 6.5;    // why 6.5?
            double spaceLeft = height - y;

            if (spaceLeft < 0)
            {
                currentPage()->growDown( -spaceLeft );
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

void EndlessPDFCreator::paintSetlist()
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

void EndlessPDFCreator::paintTableOfContents()
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

            y += lineHeight;
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

void EndlessPDFCreator::alignSongs( int mode )
{

}

void EndlessPDFCreator::optimizeForDuplex()
{

}

void EndlessPDFCreator::decoratePageNumbers()
{

}
