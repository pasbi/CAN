#include "pdfcreator.h"

#include <QPdfWriter>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>


#include "Attachments/ChordPatternAttachment/abstractchordpatternattachment.h"
#include "Attachments/ChordPatternAttachment/chordpatternattachment.h"
#include "Attachments/pdfattachment.h"
#include "Dialogs/exportpdfdialog.h"
#include "global.h"
#include "Database/EventDatabase/event.h"
#include "Database/SongDatabase/songdatabase.h"
#include "util.h"
#include "Attachments/ChordPatternAttachment/chord.h"

DEFN_CONFIG( PDFCreator, tr("PDF Export") );

#include "pdfcreatorconfig.h"

const QString PDFCreator::HYPHEN = QChar(0x2014);

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

void PDFCreator::newPage( Page::Flags flags, const QString& title = "", int i = -1 )
{
    if (i < 0)
    {
        i = m_pages.length();
    }

    m_additionalTopMargin = 0;
    m_pages.insert( i, new Page( m_baseSizeMM, title, flags ) );
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
    if (config["enable_TableOfContentsPattern"].toBool())
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
        optimizeForDuplex();
        break;
    case 4: // endless, songs will not be aligned either.
    case 5: // separate songs, will not be aligned.
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

QString labelSetlist(Setlist *setlist)
{
    QString title = QString("Setlist\n\n%1 %2 %1\n\n%3")
            .arg( QChar(0x2014) )
            .arg( setlist->event()->label() )
            .arg( QLocale().toString( setlist->event()->beginning(), QLocale().dateTimeFormat( QLocale::ShortFormat ) ) );

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

    double fontHeight = currentPainter().fontMetrics().height();
    currentPainter().drawText( QPointF(leftMargin(), topMargin() + fontHeight), label);
    m_additionalTopMargin += 2 * fontHeight;
    currentPainter().restore();
}

QString labelSong( const Song* song )
{
    QString pattern = QString("%2 %1 %3")
            .arg( PDFCreator::HYPHEN )
            .arg( song->title() )
            .arg( song->artist() );

    return pattern;
}

void PDFCreator::paintTitle()
{
    if (PDFCreator::config["enable_SongTitlePattern"].toBool())
    {
        newPage( Page::NothingSpecial );
        QString title = labelSetlist( m_setlist );
        currentPainter().drawText( pageRectMargins(), Qt::AlignCenter, title );
    }
}

void PDFCreator::insertTableOfContentsStub()
{
    m_tableOfContentsPage = currentIndex() + 1;
}

bool PDFCreator::paintSong(const Song* song)
{
    if (m_exportPDFDialog->test( song ))
    {
        newPage( Page::SongStartsHere, labelSong(song) );
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
    }
    return true;
}

void PDFCreator::paintAttachment(Attachment *attachment)
{
    if (m_exportPDFDialog->test( attachment ))
    {
        if (attachment->type() == PDFAttachment::TYPE)
        {
            paintPDFAttachment( qobject_assert_cast<PDFAttachment*>(attachment) );
        }
        else if (attachment->inherits(AbstractChordPatternAttachment::staticMetaObject.className()))
        {
            paintChordPatternAttachment( qobject_assert_cast<AbstractChordPatternAttachment*>(attachment) );
        }
    }
}

void PDFCreator::paintPDFAttachment( PDFAttachment* attachment )
{
#ifdef HAVE_POPPLER
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
#else
    Q_UNUSED( attachment );
#endif
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

void PDFCreator::paintChordPatternAttachment(AbstractChordPatternAttachment *attachment)
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
    m_currentIndex = -1;
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

QString labelTableOfContents()
{
    return QObject::tr("Setlist");
}


void PDFCreator::paintTableOfContents()
{
    typedef struct PageNumberStub {
        PageNumberStub( int page, int y ) :
            page( page ),
            y( y )
        {}
        int page;
        int y;
    } PageNumberStub;

    newPage( Page::TableOfContentsStartsHere, "", m_tableOfContentsPage );

    paintHeadline( labelTableOfContents() );

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
        double spaceLeft = pageRect().height() - bottomMargin() - lineHeight - y;
        if ( spaceLeft > 0) // content fits on page
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
                currentPage()->growDownMM( -currentPage()->painterUnitsInMM( spaceLeft ) );
            }
            else
            {
                m_tableOfContentsPage++;
                newPage( Page::NothingSpecial, "", m_tableOfContentsPage );
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
            newPage( Page::NothingSpecial, "", currentPageNum );
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
            newPage( Page::NothingSpecial, "", currentPageNum );
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

    // we have to introduce another variable because i must be reset when
    // separate pages-options is on. but i shall not be reset, therefore use j.
    int j = 0;
    for (int i = 0; i < m_pages.length(); ++i)
    {
        activatePage( i );

        if (    (currentPage()->flags() & Page::SongStartsHere       )
             && (config["AlignSongs"] == ALIGN_SONGS__SEPARATE_PAGES ) )
        {
            j = 0;
        }

        double y = pageRect().height() - bottomMargin();
        currentPainter().drawText( QRectF( 0, y - height/2, pageRect().width(), height ),
                                   QString("%1").arg( j + 1 ),
                                   QTextOption( Qt::AlignCenter )                       );
        j++;
    }
}



















QString makeFilename( QString filename )
{
    // find non-word characters and replace them by `_`
    filename.replace( QRegExp( "\\W" ), "_" );

    // replace diacritics
    filename = Util::replaceDiacritics( filename );


    //to be sure not to have forget some fancy letter, force conversion to char*
    filename = QString( filename.toStdString().c_str() );

    // now this hypothetic fancy char might turnt into a `/` or else.
    filename.replace( QRegExp( "\\W" ), "_" );

    return filename;
}

bool mkdir( const QString& path )
{
    QDir dir(path);
    if (!dir.cdUp())
    {
        return false;
    }

    QStringList tokens = path.split("/");
    QString newDir;
    while( !tokens.isEmpty() )
    {
        newDir = tokens.takeLast();
        if (!newDir.isEmpty())
        {
            break;
        }
    }

    if (newDir.isEmpty())
    {
        return false;
    }
    else
    {
        return dir.mkdir( newDir );
    }
}

void PDFCreator::paintAndSaveDocument( const Document& document, const QString& title, const QString& filename )
{

    if (document.pages.length() == 0)
    {
        qWarning() << "Empty PDF will not be created.";
        return;
    }

    QPdfWriter writer( filename );

    writer.setMargins( {0, 0, 0, 0} );
    QPageSize pageSize( document.pages[0]->sizeInMM(), QPageSize::Millimeter );     // size needs to be correctly initialized. cannot be done in loop.
    writer.setPageSize( pageSize );
    writer.setTitle( title );
    QPainter painter( &writer );

    for (int i = 0; i < document.pages.size(); ++i)
    {
        writer.setPageSizeMM( document.pages[i]->sizeInMM() );

        if (i != 0) // weird position, but this is correct.
        {
            assert( writer.newPage() );
        }

        painter.drawPicture( QPoint(0, 0), document.pages[i]->picture() );
    }

}


void PDFCreator::save(QString filename)
{
    if (config["AlignSongs"].toInt() == ALIGN_SONGS__SEPARATE_PAGES)
    {
        // each document holds exactly one song and becomes one file.
        QList<Document> documents;

        // iterate over pages to distribute them to the documents
        for (Page* page : m_pages)
        {
            if (page->flags() & Page::SongStartsHere)
            {
                documents << Document( page->title() );
            }

            if (!documents.isEmpty())   // skip table of contents, title page
            {
                documents.last().pages << page;
            }
        }

        // check whether the folder exist, create one if not.
        QFileInfo fi(filename);
        if (fi.isFile())
        {
            qWarning() << filename << "already exist and is not a directory.";
            return;
        }

        if (!QDir(filename).exists() && !mkdir(filename))
        {
            qWarning() << filename << "does not exist and cannot be created.";
            return;
        }

        // generate and save the document
        for (const Document& document : documents)
        {
            QString name = makeFilename( document.title );
            QString actualFilename = QDir( filename ).absoluteFilePath( name + ".pdf" );

            paintAndSaveDocument( document, document.title, actualFilename );
        }

    }
    else                                   // all songs in one pdf
    {
        Document document;

        document.pages = m_pages;
        QString label = "";
        if (m_setlist)
        {
            label = labelSetlist( m_setlist );
        }
        paintAndSaveDocument( document, label, filename );
    }
}









QString defaultFilename( Setlist* setlist )
{
    QString label = setlist->event()->label();
    QString type;
    switch (setlist->event()->type())
    {
    case Event::Gig:
        type = PDFCreator::tr("Gig");
        break;
    case Event::Rehearsal:
        type = PDFCreator::tr("Rehearsal");
        break;
    case Event::Other:
        type = PDFCreator::tr("Misc");
        break;
    }

    QString date = QLocale().toString( setlist->event()->beginning().date(), QLocale().dateFormat( QLocale::ShortFormat ) ).replace(".", "_");

    if (label.isEmpty())
    {
        label = type;
    }

    if (date.isEmpty())
    {
        return label;
    }
    else
    {
        return label + "__" + date;
    }
}

// static helper
QString PDFCreator::setlistFilename( QWidget* parent, Setlist* setlist, bool separatePages )
{

    QString filename = config["DefaultPDFSavePath"].toString();
    filename = QDir(filename).absoluteFilePath( defaultFilename( setlist ) );

    // QFileDialog::getSaveFilename does not asks for overwriting files when the default filename is used. Workaround: Disable overwrite
    // confirmation for all files and ask for it in an other dialog.
    bool allowWriteFile;
    do
    {
        if (separatePages)
        {
            filename = QFileDialog::getExistingDirectory(   parent,
                                                            tr("Export PDF ..."),
                                                            filename,
                                                            QFileDialog::DontConfirmOverwrite );
        }
        else
        {
            filename = QFileDialog::getSaveFileName(    parent,
                                                        tr("Export PDF ..."),
                                                        filename,
                                                        "*.pdf",
                                                        NULL,
                                                        QFileDialog::DontConfirmOverwrite );
        }

        if (filename.isEmpty())
        {
            break;  // putting this break in while( ... ), means complicating the whole thing since it will be
                    // expanded to *.pdf in the following and thus would not be empty
        }
        if (!separatePages)
        {
            if (!filename.endsWith(".pdf"))
            {
                filename.append(".pdf");
            }
        }

        allowWriteFile = !QFileInfo(filename).exists();
        if ( !separatePages && !filename.isEmpty() && !allowWriteFile )
        {
            if (QMessageBox::question( parent,
                                       tr("Confirm overwrite"),
                                       QString(tr( "%1 already exists.\n"
                                                   "Do you want to replace it?" )).arg(filename),
                                       QMessageBox::Yes | QMessageBox::No,
                                       QMessageBox::Yes )
                    == QMessageBox::Yes )
            {
                allowWriteFile = true;
            }
        }

        if (separatePages)
        {
            allowWriteFile = true;  // always write to existing dirs.
        }

    }
    while ( !allowWriteFile );     // filename is not in use by now or user allows to overwrite.
                                   // note the break if filename is empty.

    return filename;
}


void PDFCreator::exportSetlist( Setlist* setlist, QWidget* widgetParent )
{
    ExportPDFDialog* exportPDFDialog = new ExportPDFDialog( widgetParent );

    if (exportPDFDialog->exec() == QDialog::Accepted)
    {
        bool askForDirectory = (config["AlignSongs"].toInt() == ALIGN_SONGS__SEPARATE_PAGES);
        QString filename = setlistFilename( widgetParent, setlist, askForDirectory );

        if (!filename.isEmpty())
        {
            QFileInfo fi(filename);

            if (fi.exists() && !fi.isReadable())
            {
                QMessageBox::warning( widgetParent,
                                      tr("Cannot write"),
                                      QString(tr("File %1 is not writable.").arg(filename)),
                                      QMessageBox::Ok,
                                      QMessageBox::NoButton );
            }
            else
            {
                config.set( "DefaultPDFSavePath", QFileInfo( filename ).path() );
                PDFCreator pdfCreator( QPageSize::size( QPageSize::A4, QPageSize::Millimeter ), setlist, filename );
                pdfCreator.m_exportPDFDialog = exportPDFDialog;

                QProgressDialog dialog;
                dialog.setRange(0, setlist->items().length());
                connect( &pdfCreator, SIGNAL(progress(int)), &dialog, SLOT(setValue(int)) );
                connect( &pdfCreator, SIGNAL(currentTaskChanged(QString)), &dialog, SLOT(setLabelText(QString)) );
                connect( &pdfCreator, SIGNAL(finished()), &dialog, SLOT(close()) );
                connect( &dialog, &QProgressDialog::canceled, [&pdfCreator]()
                {
                    pdfCreator.requestInterruption();
                });
                pdfCreator.start();

                dialog.exec();
                pdfCreator.wait();
            }
        }
    }

    delete exportPDFDialog;
    exportPDFDialog = NULL;
}

void PDFCreator::paintChordPatternAttachment(AbstractChordPatternAttachment *attachment, const QString &path)
{
    // save config and replace it at the end.
    Configurable savedConfigs = config;

    config.set("enable_TitlePagePattern", false);
    config.set("enable_SongTitlePattern", false);
    config.set("PDFSize", 4); // DinA4
    config.set("AlignSongs", 4); // we always want to have one page per song.
    config.set("enable_TableOfContentsPattern", false);
    config.set("PageNumbers", false);

    PDFCreator creator( QPageSize::size( QPageSize::A4, QPageSize::Millimeter ), NULL, "" );
    creator.m_currentIndex = -1;
    creator.newPage( Page::SongStartsHere );
    creator.paintChordPatternAttachment( attachment );
    creator.save( path );

    // restore preference
    config = savedConfigs;
}
























