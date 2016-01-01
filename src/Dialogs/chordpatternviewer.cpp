#include "chordpatternviewer.h"
#include "ui_chordpatternviewer.h"
#include <QMessageBox>
#include <PDFCreator/pdfcreator.h>
#include <QTimer>
#include <QMouseEvent>
#include <QKeyEvent>
#include "Program/midicontroller.h"
#include "Attachments/ChordPatternAttachment/abstractchordpatternattachment.h"
#include "poppler.h"
#include "Database/SongDatabase/song.h"
#include "overlaydecorator.h"
#include "Program/midicommand.h"
#include <QScrollBar>
#include "Attachments/AudioAttachment/audioattachment.h"



int describedWidth(const QImage& image, const QColor& backgroundColor)
{
    for (int x = image.width() - 1; x >= 0; --x)
    {
        for (int y = 0; y < image.height(); ++y)
        {
            if (image.pixel(x, y) != backgroundColor.rgba())
            {
                return x;
            }
        }
    }
    return 0;
}

ChordPatternViewer::ChordPatternViewer(AbstractChordPatternAttachment *attachment, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChordPatternViewer),
    m_attachment( attachment ),
    m_pos( 0 )
{
    ui->setupUi(this);
    m_playTimer = new QTimer( this );
    m_speed = attachment->scrollDownTempo();
    setWindowState( Qt::WindowFullScreen );

#ifdef HAVE_POPPLER
    Poppler::Document* document = nullptr;
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

    m_zoom = preference<double>("ChordPatternViewZoom");

    QTimer::singleShot( 1, this, SLOT(applyZoom()) );

    applySpeed();

    m_playTimer->setInterval( 50 );
    connect( m_playTimer, SIGNAL(timeout()), this, SLOT(on_playTimerTimeout()) );

#else
    QMessageBox::information( this,
                              tr("Poppler is not available"),
                              tr("It seems this application was build without poppler.\n"
                                 "Thus, this feature is not available."));
    QTimer::singleShot( 1, this, SLOT(reject()) );
#endif

    int channel = preference<int>("Channel") - 1;
    if (channel >= 0)
    {
        MidiCommand::defaultChannel = (MidiCommand::Channel) (channel);
        const Program& program = attachment->song()->program();
        if (program.isValid())
        {
            MidiController::singleton()->send( program );
        }
    }

    ui->buttonColumnCount->blockSignals(true);
    ui->buttonColumnCount->setChecked( preference<bool>("ChordPatternViewTwoColumn") );
    ui->buttonColumnCount->blockSignals(false);

    ui->audioPlayerWidget->hide();
    initializeAudioPlayerWidget();
}

ChordPatternViewer::~ChordPatternViewer()
{
    setPreference( "ChordPatternViewZoom", m_zoom );
    delete ui;
}

void ChordPatternViewer::displayChordPatternAttachment(AbstractChordPatternAttachment *attachment, QWidget* parent)
{
    ChordPatternViewer cpv( attachment, parent );
    cpv.exec();

    if (qFuzzyIsNull(attachment->scrollDownTempo()))
    {
        attachment->setScrollDownTempo( cpv.scrollDownTempo() );
    }
    else if (cpv.scrollDownTempo() != attachment->scrollDownTempo())
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
    QDialog::resizeEvent(e);
}

int ChordPatternViewer::pdfWidth()
{
    return ui->scrollArea->viewport()->width() * m_zoom;
}

static const int TWO_PAGE_GAP = 30;

int twoPageViewWidth( int describedWidth )
{
    return 2*describedWidth + TWO_PAGE_GAP + TWO_PAGE_GAP/2;
}

QPixmap twoPageView( QPixmap pixmap, int describedWidth, int viewportHeight, int height )
{
    QSize size = QSize( twoPageViewWidth(describedWidth), height );
    QPixmap doublePixmap( size );
    doublePixmap.fill( Qt::white );

    QPainter painter;
    painter.begin(&doublePixmap);
    painter.drawPixmap( 0,                               0,               pixmap );
    painter.drawPixmap( describedWidth + TWO_PAGE_GAP/2, -viewportHeight, pixmap );

    QPen pen;
    pen.setWidthF( 3 );
    painter.setPen( pen );
    painter.drawLine( QPoint(describedWidth + TWO_PAGE_GAP/2, 0), QPoint(describedWidth + TWO_PAGE_GAP/2, doublePixmap.height()) );

    return doublePixmap;
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
    int describedWidth = ::describedWidth(pixmap.toImage(), Qt::white);

    if ( preference<bool>("ChordPatternViewTwoColumn"))
    {
        pixmap = twoPageView(pixmap, describedWidth, ui->scrollArea->viewport()->height(), pixmap.height());
    }
    else
    {
        // keep pixmap
    }

    ui->label->setPixmap( pixmap );

    qDebug() << m_zoom;
}

void ChordPatternViewer::on_buttonZoomOut_clicked()
{
    m_pos /= 1.02;
    m_zoom /= 1.02;
    applyZoom();
}

void ChordPatternViewer::on_buttonZoomIn_clicked()
{
    m_pos *= 1.02;
    m_zoom *= 1.02;
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

    ui->label->setOverlayText( QString("%1").arg(qRound(m_speed * 1000) / 1000.0 ) );
}

void ChordPatternViewer::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Space)
    {
        ui->pushButtonPlay->toggle();
    }
    else if (e->key() == Qt::Key_Up)
    {
        scrollUp();
    }
    else if (e->key() == Qt::Key_Down)
    {
        scrollDown();
    }
    else if (e->key() == Qt::Key_Plus)
    {
        if (!e->isAutoRepeat())
        {
            on_buttonZoomIn_clicked();
        }
    }
    else if (e->key() == Qt::Key_Minus)
    {
        if (!e->isAutoRepeat())
        {
            on_buttonZoomOut_clicked();
        }
    }
}


void ChordPatternViewer::scrollDown()
{
    m_pos = qMin( (double) ui->label->height(), m_pos + 10 * m_zoom );
    update();
}

void ChordPatternViewer::scrollUp()
{
    m_pos = qMax(0.0, m_pos - 10 * m_zoom );
    update();
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
    ui->scrollArea->ensureVisible( ui->label->width() / 2, m_pos, 0, ui->scrollArea->viewport()->height() / 2 );
    ui->label->setLinePos( m_pos );
    QDialog::update();
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

void ChordPatternViewer::wheelEvent(QWheelEvent *e)
{
    if (e->delta() > 0)
    {
        scrollUp();
    }
    else
    {
        scrollDown();
    }
}

void ChordPatternViewer::on_buttonAutoZoom_clicked()
{
    if (preference<bool>("ChordPatternViewTwoColumn"))
    {
        m_zoom = (double) m_pixmap.width() / (twoPageViewWidth(describedWidth(m_pixmap.toImage(), Qt::white)) + 10);
    }
    else
    {
        m_zoom = 1;
    }
    applyZoom();
}

void ChordPatternViewer::on_buttonColumnCount_toggled(bool checked)
{
    setPreference("ChordPatternViewTwoColumn", checked);
    applyZoom();
}

void ChordPatternViewer::initializeAudioPlayerWidget()
{
    AudioAttachment* firstAudioAttachment = nullptr;
    connect(ui->buttonSelectAudioAttachment, SIGNAL(clicked(bool)), ui->audioPlayerWidget, SLOT(setVisible(bool)));
    for (Attachment* a : m_attachment->song()->attachments())
    {
        if (a->inherits("AudioAttachment"))
        {
            AudioAttachment* aa = static_cast<AudioAttachment*>(a);
            if (!firstAudioAttachment)
            {
                firstAudioAttachment = aa;
            }
            QAction* action = new QAction(ui->buttonSelectAudioAttachment);
            action->setText(a->name());
            ui->buttonSelectAudioAttachment->addAction(action);
            connect(action, &QAction::triggered, [this, aa]()
            {
                aa->open();
                ui->audioPlayerWidget->setPlayer(&aa->player());
            });
        }
    }

    if (ui->buttonSelectAudioAttachment->actions().isEmpty())
    {
        ui->buttonSelectAudioAttachment->setEnabled(false);
    }

    if (Player::activePlayer())
    {
        ui->audioPlayerWidget->setPlayer(Player::activePlayer());
    }
    else if (firstAudioAttachment)
    {
        firstAudioAttachment->open();
        ui->audioPlayerWidget->setPlayer(&firstAudioAttachment->player());
    }
    else
    {
        ui->audioPlayerWidget->setPlayer(nullptr);
    }

}

void ChordPatternViewer::showEvent(QShowEvent *e)
{
    on_buttonAutoZoom_clicked();
    double minZoom = preference<double>("minZoom");
    double maxZoom = preference<double>("maxZoom");
    m_zoom = qBound(minZoom, m_zoom, maxZoom);    // too deep zooms are not convenient
    applyZoom();

    QDialog::showEvent(e);
}
