#include "chordpatternviewer.h"
#include "ui_chordpatternviewer.h"
#include <QMessageBox>
#include <PDFCreator/pdfcreator.h>
#include <QTimer>

DEFN_CONFIG( ChordPatternViewer, "ChordPatternViewer" );
CONFIGURABLE_ADD_ITEM_HIDDEN( ChordPatternViewer, zoom, 1.0 );

ChordPatternViewer::ChordPatternViewer(ChordPatternAttachment* attachment, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChordPatternViewer),
    m_attachment( attachment )
{
    ui->setupUi(this);

    Poppler::Document* document = NULL;
    { // open pdf document
        QTemporaryFile file;
        file.open();
        PDFCreator::paintChordPatternAttachment( attachment, file.fileName() );
        document = Poppler::Document::load( file.fileName() );
    }

    { // display pdf
    if (!document)
    {
        qWarning() << "document does not exist.";
        return;
    }

    Poppler::Page* page;
    if ( document->numPages() < 1 )
    {
        qWarning() << "no page in document.";
        return;
    }
    else if (document->numPages() > 1)
    {
        qWarning() << "multiple pages in document. Displaying only first one.";
    }

    page = document->page( 0 );
    m_pixmap = QPixmap::fromImage( page->renderToImage( 300, 300 ) );

    ui->label->setPixmap( m_pixmap );
    }

    m_zoom = config["zoom"].toDouble();
    m_speed = attachment->scrollDownTempo();

    QTimer::singleShot( 1, this, SLOT(applyZoom()) );

//    setWindowState( Qt::WindowFullScreen );
    m_hud = new HUD(this);
    m_hud->hide();
}

ChordPatternViewer::~ChordPatternViewer()
{
    config.set( "zoom", m_zoom );
    delete ui;
}

void ChordPatternViewer::displayChordPatternAttachment(ChordPatternAttachment *attachment, QWidget* parent)
{
    ChordPatternViewer cpv( attachment, parent );
    cpv.exec();

    if (cpv.scrollDownTempo() != attachment->scrollDownTempo())
    {
        if ( QMessageBox::question( parent,
                                    tr("Apply new tempo?"),
                                    tr("You changed the tempo of this chord pattern.\n"
                                       "Do you want to keep it?"),
                                    QMessageBox::Yes | QMessageBox::No,
                                    QMessageBox::Yes )
             == QMessageBox::Yes )
        {
            attachment->setScrollDownTempo( cpv.scrollDownTempo() );
        }
    }
}

void ChordPatternViewer::resizeEvent(QResizeEvent *e)
{
    applyZoom();
    m_hud->replace();
    QDialog::resizeEvent(e);
}

int ChordPatternViewer::pdfWidth()
{
    return ui->scrollArea->viewport()->width() * m_zoom;
}

void ChordPatternViewer::applyZoom()
{
    if (m_zoom <= 0.05)
    {
        m_zoom = 0.05;
        ui->buttonZoomOut->setEnabled( false );
    }
    else
    {
        ui->buttonZoomOut->setEnabled( true );
    }

    if (m_zoom >= 1)
    {
        m_zoom = 1;
        ui->buttonZoomIn->setEnabled( false );
    }
    else
    {
        ui->buttonZoomIn->setEnabled( true );
    }

    QPixmap pixmap = m_pixmap.scaledToWidth( pdfWidth(), Qt::SmoothTransformation );
    ui->label->setPixmap( pixmap  );
}

void ChordPatternViewer::on_buttonZoomOut_clicked()
{
    m_zoom /= 1.05;
    applyZoom();
}

void ChordPatternViewer::on_buttonZoomIn_clicked()
{
    m_zoom *= 1.05;
    applyZoom();
}


void ChordPatternViewer::on_buttonFaster_clicked()
{
    m_speed *= 1.05;
    applySpeed();
}

void ChordPatternViewer::on_buttonSlower_clicked()
{
    m_speed /= 1.05;
    applySpeed();
}

void ChordPatternViewer::applySpeed()
{
    if (m_speed >= 100)
    {
        m_speed = 100;
        ui->buttonFaster->setEnabled( false );
    }
    else
    {
        ui->buttonFaster->setEnabled( true );
    }

    if (m_speed <= 0.1)
    {
        m_speed = 0.1;
        ui->buttonSlower->setEnabled( false );
    }
    else
    {
        ui->buttonSlower->setEnabled( true );
    }

    m_hud->setText( QString("%1").arg(qRound(m_speed * 100) / 100.0) );
    m_hud->show();
}






