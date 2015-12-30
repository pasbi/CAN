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
#include "chord.h"
#include "Database/EventDatabase/setlist.h"

#include "pdfcreatorconfig.h"

const QString PDFCreator::HYPHEN = QChar(0x2014);
const int PDFCreator::ALIGN_SONGS__SEPARATE_PAGES = 5;


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

void PDFCreator::newPage(Page::Flags flags, const QString& title, int index )
{
    if (index < 0)
    {
        index = m_pages.length();
    }

    Page* page = new Page( m_baseSizeMM, title, flags );
    page->setAdditionalTopMargin(0);
    m_pages.insert( index, page );
    activatePage( index );
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

Page* PDFCreator::currentPage() const
{
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
    if (app().preference<bool>("enable_TableOfContentsPattern"))
    {
        paintTableOfContents();
    }

    if (isInterruptionRequested())
    {
        return;
    }

    notifyCurrentTaskChanged( tr("Align songs") );
    switch (app().preference<int>("AlignSongs"))
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
    if (app().preference<bool>("PageNumbers"))
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


bool PDFCreator::isEndlessPage() const
{
    return app().preference<int>("AlignSongs") == 4;
}


//////////////////////////////////////////////
////
///  pdf paint member
//
/////////////////////////////////////////////

void PDFCreator::paintHeadline(Page* page, const QString& label)
{
    QPainter* painter = page->painter();
    painter->save();

    QFont font = painter->font();
    font.setBold( true );
    font.setPointSizeF( 15 );
    font.setFamily( "lucida" );
    painter->setFont( font );

    double fontHeight = painter->fontMetrics().height();
    painter->drawText( QPointF(page->leftMargin(), page->topMargin() + fontHeight), label);
    page->setAdditionalTopMargin(2*fontHeight);
    painter->restore();
}

QString labelSong( const Song* song )
{
    QString pattern = QString("%2 %1 %3")
            .arg( PDFCreator::HYPHEN )
            .arg( song->title() )
            .arg( song->artist() );

    return pattern;
}

bool PDFCreator::paintSong(const Song* song)
{
    newPage( Page::SongStartsHere, labelSong(song) );
    QString headline = labelSong( song );
    m_tableOfContents.append( headline );
    paintHeadline( currentPage(), headline );

    int i = 0;
    for ( Attachment* attachment : song->attachments() )
    {
        if (isInterruptionRequested()) return false;
        notifyCurrentTaskChanged( QString(tr("Draw attachment %1 of song %2"))
                                      .arg(song->title())
                                      .arg(attachment->name())                  );

        if (attachment->isPaintable())
        {
            if (i++ != 0)
            {
                newPage( Page::NothingSpecial );
            }
            currentPage()->painter()->save();
            attachment->paint(this);
            currentPage()->painter()->restore();
        }
    }
    return true;
}

bool PDFCreator::pageBreak( const QStringList & lines, const int currentLine, const double heightLeft, const QPainter* painter )
{
    // return whether we must use another page to fit the content
    if (lines[currentLine].isEmpty())
    {
        // we are currently at a paragraph break
        // if next paragraph fits, return false, true otherwise.
        double paragraphHeight = 0;
        double lineHeight =  painter->fontMetrics().height();
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
                    paragraphHeight += lineHeight * app().preference<double>("ChordLineSpacing");
                }
                else
                {
                    paragraphHeight += lineHeight * app().preference<double>("LineSpacing");
                }
            }
            paragraph += lines[i] + "\n";
        }

        return paragraphHeight > heightLeft;
    }
    else
    {
        // we are not at a paragraph break. break if the current line will not fit anymore.
        return painter->fontMetrics().height() > heightLeft;
    }
}

void PDFCreator::drawContinueOnNextPageMark(const Page* page, QPainter *painter)
{
    painter->save();
    QFont font = painter->font();
    font.setBold( true );
    font.setPointSizeF( font.pointSizeF() * 3 );
    painter->setFont( font );
    QTextOption option;
    option.setAlignment( Qt::AlignBottom | Qt::AlignRight );
    painter->drawText( page->contentRect(), QChar(0x293E), option );
    painter->restore();
}

QString labelSetlist(Setlist *setlist)
{
    QString title = QString("Setlist\n\n%1 %2 %1\n\n%3")
            .arg( QChar(0x2014) )
            .arg( setlist->event()->label() )
            .arg( QLocale().toString( setlist->event()->beginning(), QLocale().dateTimeFormat( QLocale::ShortFormat ) ) );

    return title;
}

void PDFCreator::paintSetlist()
{
    m_currentIndex = -1;

    newPage( Page::NothingSpecial );
    QString title = labelSetlist( m_setlist );
    currentPage()->painter()->drawText( currentPage()->contentRect(), Qt::AlignCenter, title );

    m_tableOfContentsPage = currentIndex() + 1;

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

static void configurePainter(QPainter* painter)
{
    QFont font = painter->font();
    font.setBold( true );
    font.setFamily( "lucida" );
    painter->setFont( font );
}

#define GET_PAINTER painter = currentPage()->painter(); configurePainter(painter); painter->save();
#define RESTORE_PAINTER painter->restore();

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

    paintHeadline( currentPage(), labelTableOfContents() );

    // draw entries and find places to fill in page numbers
    QList<PageNumberStub> pageNumberStubs;

    QPainter* GET_PAINTER;
    double y = currentPage()->topMargin();
    const double lineHeight = painter->fontMetrics().height();
    while (!m_tableOfContents.isEmpty())
    {
        double spaceLeft = currentPage()->rect().height() - currentPage()->bottomMargin() - lineHeight - y;
        if ( spaceLeft > 0) // content fits on page
        {
            QString currentEntry = m_tableOfContents.takeFirst();
            painter->drawText( QPointF( currentPage()->leftMargin(), y), currentEntry  );
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
                RESTORE_PAINTER;
                newPage( Page::NothingSpecial, "", m_tableOfContentsPage );
                GET_PAINTER;
                y = currentPage()->topMargin();
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
        RESTORE_PAINTER;
        activatePage( stub.page );
        GET_PAINTER;
        double textWidth = painter->fontMetrics().boundingRect( text ).width();
        double x = currentPage()->contentRect().right() - textWidth;
        painter->drawText( QPointF( x, stub.y), text );

    }
    RESTORE_PAINTER;
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

    // we have to introduce another variable because i must be reset when
    // separate pages-options is on. but i shall not be reset, therefore use j.
    int j = 0;
    for (int i = 0; i < m_pages.length(); ++i)
    {
        activatePage( i );
        QPainter* painter = currentPage()->painter();
        double height = painter->fontMetrics().height();

        if (    (currentPage()->flags() & Page::SongStartsHere       )
             && (app().preference<int>("AlignSongs") == ALIGN_SONGS__SEPARATE_PAGES ) )
        {
            j = 0;
        }

        double y = currentPage()->rect().height() - currentPage()->bottomMargin();
        painter->drawText( QRectF( 0, y - height/2, currentPage()->rect().width(), height ),
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
    if (app().preference<int>("AlignSongs") == ALIGN_SONGS__SEPARATE_PAGES)
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
    QString type = Event::eventTypeName(setlist->event()->type());

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

    QString filename = app().preference<QString>("DefaultPDFSavePath");
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
                                                        nullptr,
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
        bool askForDirectory = (app().preference<int>("AlignSongs") == ALIGN_SONGS__SEPARATE_PAGES);
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
                app().setPreference( "DefaultPDFSavePath", QFileInfo( filename ).path() );
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
    exportPDFDialog = nullptr;
}

void PDFCreator::paintChordPatternAttachment(AbstractChordPatternAttachment *attachment, const QString &path)
{
    // save config and replace it at the end.
    app().preferences()->save();

    app().setPreference("enable_TitlePagePattern", false);
    app().setPreference("PDFSize", 4); // DinA4
    app().setPreference("AlignSongs", 4); // we always want to have one page per song.
    app().setPreference("enable_TableOfContentsPattern", false);
    app().setPreference("PageNumbers", false);

    PDFCreator creator( QPageSize::size( QPageSize::A4, QPageSize::Millimeter ), nullptr, "" );
    creator.m_currentIndex = -1;
    creator.newPage( Page::SongStartsHere );
    attachment->paint(&creator);
    creator.save( path );

    // restore preference
    app().preferences()->restore();
}
























