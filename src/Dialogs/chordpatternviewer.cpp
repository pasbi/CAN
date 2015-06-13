#include "chordpatternviewer.h"
#include "ui_chordpatternviewer.h"
#include <QMessageBox>
#include <PDFCreator/pdfcreator.h>
#include <QTimer>
#include <QMouseEvent>
#include <QKeyEvent>

DEFN_CONFIG( ChordPatternViewer, "ChordPatternViewer" );
CONFIGURABLE_ADD_ITEM_HIDDEN( ChordPatternViewer, zoom, 1.0 );
CONFIGURABLE_ADD_ITEM_HIDDEN( ChordPatternViewer, line, true );

ChordPatternViewer::ChordPatternViewer(ChordPatternAttachment* attachment, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChordPatternViewer),
    m_attachment( attachment ),
    m_pos( 0 )
{
    ui->setupUi(this);
    m_hud = new HUD(this);
    m_playTimer = new QTimer( this );
    m_speed = attachment->scrollDownTempo();
    setWindowState( Qt::WindowFullScreen );

#ifdef HAVE_POPPLER
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

    QTimer::singleShot( 1, this, SLOT(applyZoom()) );

    applySpeed();
    m_hud->hide();

    ui->label->installEventFilter( this );
    ui->scrollArea->installEventFilter( this );
    ui->scrollAreaWidgetContents->installEventFilter( this );

    m_playTimer->setInterval( 50 );
    connect( m_playTimer, SIGNAL(timeout()), this, SLOT(on_playTimerTimeout()) );

    ui->buttonEnableLine->setChecked( config["line"].toBool() );
#else
    QMessageBox::information( this,
                              tr("Poppler is not available"),
                              tr("It seems this application was build without poppler.\n"
                                 "Thus, this feature is not available."));
    QTimer::singleShot( 1, this, SLOT(reject()) );
#endif
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
    m_pos /= 1.05;
    m_zoom /= 1.05;
    applyZoom();
}

void ChordPatternViewer::on_buttonZoomIn_clicked()
{
    m_pos *= 1.05;
    m_zoom *= 1.05;
    applyZoom();
}


void ChordPatternViewer::on_buttonFaster_clicked()
{
    m_speed *= 1.02;
    applySpeed();
}

void ChordPatternViewer::on_buttonSlower_clicked()
{
    m_speed /= 1.02;
    applySpeed();
}

void ChordPatternViewer::applySpeed()
{
    if (m_speed >= 10)
    {
        m_speed = 10;
        ui->buttonFaster->setEnabled( false );
    }
    else
    {
        ui->buttonFaster->setEnabled( true );
    }

    if (m_speed <= 0.05)
    {
        m_speed = 0.05;
        ui->buttonSlower->setEnabled( false );
    }
    else
    {
        ui->buttonSlower->setEnabled( true );
    }

    m_hud->setText( QString("%1").arg(qRound(m_speed * 1000) / 1000.0) );

    m_hud->show();
}

bool ChordPatternViewer::eventFilter(QObject *o, QEvent *e)
{
    if (o == ui->label && e->type() == QEvent::Paint)
    {
        QLabel* label = (QLabel*) o;
        QPainter painter(label);
        const QPixmap* pixmap = label->pixmap();
        int xshift = ui->scrollArea->viewport()->width() - pixmap->width();
        painter.drawPixmap( xshift/2, 0, *pixmap );
        painter.setPen( QColor(255, 0, 0, 100));
        if (config["line"].toBool())
        {
            painter.drawLine( 0, m_pos, label->width(), m_pos );
        }
        return true;
    }
    // catch the events before QWidget::keyEvent has chance.
    else if ( e->type() == QEvent::KeyPress )
    {
        QKeyEvent* ke = (QKeyEvent*) e;
        if (ke->key() == Qt::Key_Space)
        {
            ui->pushButtonPlay->toggle();
        }
        else if (ke->key() == Qt::Key_Up)
        {
            m_pos = qMax(0.0, m_pos - 10 * m_zoom );
            update();
            return true;
        }
        else if (ke->key() == Qt::Key_Down)
        {
            m_pos = qMin( (double) ui->label->height(), m_pos + 20 * m_zoom );
            update();
            return true;
        }
        else
        {
            return QDialog::eventFilter(o, e);
        }
    }
    return QDialog::eventFilter(o, e);

}


void ChordPatternViewer::on_playTimerTimeout()
{
    m_pos += m_speed * m_zoom;
    if (m_pos >= ui->label->height())
    {
        m_atEnd = true;
        m_playTimer->stop();
        ui->pushButtonPlay->setChecked(false);
    }
    update();
}

void ChordPatternViewer::on_pushButtonPauseJumpToBegin_clicked()
{
    m_playTimer->stop();
    ui->pushButtonPlay->setChecked(false);
    m_pos = 0;
    update();
}

void ChordPatternViewer::update()
{
    double pos = m_pos;
    ui->scrollArea->ensureVisible( ui->label->width() / 2, pos, 0, ui->scrollArea->viewport()->height() / 2 );
    QDialog::update();
}

void ChordPatternViewer::on_buttonEnableLine_clicked(bool checked)
{
    config.set("line", checked);
    update();
}

void ChordPatternViewer::on_pushButtonPlay_toggled(bool checked)
{
    if (checked)
    {
        m_playTimer->start();
    }
    else
    {
        m_playTimer->stop();
    }
}
