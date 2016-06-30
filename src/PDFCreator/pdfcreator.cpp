#include "pdfcreator.h"

#include <QPdfWriter>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>

#include "Database/EventDatabase/setlist.h"
#include "Database/EventDatabase/event.h"


PDFCreator::PDFCreator(QSizeF baseSizeMM, Setlist* setlist, const QString &filename) :
    AbstractRenderer(baseSizeMM, setlist, filename)
{
}

PDFCreator::~PDFCreator()
{
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
            Q_ASSERT( writer.newPage() );
        }

        painter.drawPicture( QPoint(0, 0), document.pages[i]->picture() );
    }
}

void PDFCreator::save(QString filename)
{
    if (preference<int>("AlignSongs") == ALIGN_SONGS__SEPARATE_PAGES)
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
    QString label = setlist->event()->attributeDisplay("label");
    QString type = setlist->event()->attributeDisplay("type");

    QString date = setlist->event()->attributeDisplay("beginning");

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

    QString filename = preference<QString>("DefaultPDFSavePath");
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
    bool askForDirectory = (preference<int>("AlignSongs") == ALIGN_SONGS__SEPARATE_PAGES);
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
            setPreference( "DefaultPDFSavePath", QFileInfo( filename ).path() );
            PDFCreator ar( QPageSize::size( QPageSize::A4, QPageSize::Millimeter ), setlist, filename );

            QProgressDialog dialog;
            dialog.setRange(0, setlist->items().length());
            connect( &ar, SIGNAL(progress(int)), &dialog, SLOT(setValue(int)) );
            connect( &ar, SIGNAL(currentTaskChanged(QString)), &dialog, SLOT(setLabelText(QString)) );
            connect( &ar, SIGNAL(finished()), &dialog, SLOT(close()) );
            connect( &dialog, &QProgressDialog::canceled, [&ar]()
            {
                ar.requestInterruption();
            });
            ar.start();

            dialog.exec();
            ar.wait();
        }
    }
}

void PDFCreator::run()
{
    AbstractRenderer::run();

    notifyCurrentTaskChanged( tr("Save PDF") );
    save(m_filename);
}
