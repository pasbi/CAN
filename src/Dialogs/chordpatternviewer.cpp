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

    QTimer::singleShot( 1, this, SLOT(adjustPDFSize()) );
    ui->sliderSize->setValue( config["zoom"].toDouble() * ui->sliderSize->maximum() );
    connect( ui->sliderSize, SIGNAL(valueChanged(int)), this, SLOT(adjustPDFSizeLater()) );

    setWindowState( Qt::WindowFullScreen );
}

ChordPatternViewer::~ChordPatternViewer()
{
    config.set("zoom", (double) ui->sliderSize->value() / ui->sliderSize->maximum());
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
    adjustPDFSize();
    QDialog::resizeEvent(e);
}

void ChordPatternViewer::adjustPDFSizeLater()
{
    adjustPDFSize();
}

void ChordPatternViewer::adjustPDFSize()
{
    QPixmap pixmap = m_pixmap.scaledToWidth( pdfWidth(), Qt::SmoothTransformation );
    ui->label->setPixmap( pixmap  );
}

int ChordPatternViewer::pdfWidth()
{
    double v = ui->sliderSize->value();
    double max = ui->sliderSize->maximum();
    return ui->scrollArea->viewport()->width() * (double) v / max;
}
