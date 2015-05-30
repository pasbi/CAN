#include "pdfattachmentview.h"
#include "ui_pdfattachmentview.h"
#include "Attachments/pdfattachment.h"
#include <QPainter>
#include <qmath.h>
#include <QTimer>
#include "configurable.h"

#include "global.h"

DEFN_CREATABLE(PDFAttachmentView, AttachmentView);
DEFN_CONFIG( PDFAttachmentView, "PDF" );

CONFIGURABLE_ADD_ITEM( PDFAttachmentView,
                       Quality,
                       QT_TRANSLATE_NOOP("ConfigurableItem", "Quality"),
                       72.0,
                       ConfigurationItemOptions::AdvancedDoubleSliderOptions( 0,
                                                                              100,
                                                                              1,
                                                                              QT_TRANSLATE_NOOP("ConfigurableItem", "dpi")
                                                                              )
                     );


PDFAttachmentView::PDFAttachmentView(QWidget *parent) :
    IndexedFileAttachmentView(parent),
    ui(new Ui::PDFAttachmentView)
{
    m_pdfWidget = new QWidget(this);
    ui->setupUi(m_pdfWidget);
    setWidget( m_pdfWidget );

    // propably because of ui is not set up on this but on m_pdfWidget, connect-by-name does not work.
    connect(ui->buttonZoomIn,       SIGNAL(clicked()),              this, SLOT(on_buttonZoomIn_clicked())           );
    connect(ui->buttonZoomOut,      SIGNAL(clicked()),              this, SLOT(on_buttonZoomOut_clicked())          );
    connect(ui->spinBoxScale,       SIGNAL(editingFinished()),      this, SLOT(on_spinBoxScale_editingFinished())   );
    ui->spinBoxScale->installEventFilter( this );
    connect(ui->spinBoxScale,       SIGNAL(editingFinished()),      this, SLOT(on_spinBoxScale_editingFinished())   );
    connect(ui->buttonNextPage,     SIGNAL(clicked()),              this, SLOT(on_buttonNextPage_clicked())         );
    connect(ui->buttonPreviousPage, SIGNAL(clicked()),              this, SLOT(on_buttonPreviousPage_clicked())     );
    connect(ui->spinBoxPage,        SIGNAL(valueChanged(int)),      this, SLOT(on_spinBoxPage_valueChanged(int))    );

    QTimer::singleShot(1, this, SLOT(open()));  // wait until scroll area has appropriate size
}

PDFAttachmentView::~PDFAttachmentView()
{
    delete ui;
}

bool PDFAttachmentView::eventFilter(QObject *o, QEvent *e)
{
    bool r = IndexedFileAttachmentView::eventFilter(o, e);
    if (o == ui->spinBoxScale && e->type() == QEvent::MouseButtonRelease)
    {
        on_spinBoxScale_editingFinished();
    }

    return r;
}

void PDFAttachmentView::polish()
{
    IndexedFileAttachmentView::polish();
    open();
}

void PDFAttachmentView::open()
{
    IndexedFileAttachmentView::open();
    handlePageControlEnabled();

    PDFAttachment* pdf = attachment<PDFAttachment>();
    Poppler::Document* doc = pdf ? pdf->document() : NULL;
    if (!doc)
    {
        ui->label->setPixmap(QPixmap());
    }
    else
    {
        m_currentPage = qMin( m_currentPage, doc->numPages() - 1);

        int size = ui->scrollArea->width();

        double quality = config.value("Quality").toDouble();
        QImage image = doc->page(m_currentPage)->renderToImage( quality, quality );

        image = image.scaledToWidth( size * m_zoom - 10 );

        ui->label->setPixmap(QPixmap::fromImage(image));
    }
}

void PDFAttachmentView::on_buttonZoomIn_clicked()
{
    ui->spinBoxScale->setValue( m_zoom * 1.125 * 100 );
    m_zoom = qRound( ui->spinBoxScale->value() * 100.0 ) / 100.0 / 100.0;
    open();
}

void PDFAttachmentView::on_buttonZoomOut_clicked()
{
    ui->spinBoxScale->setValue( m_zoom / 1.125 * 100 );
    m_zoom = qRound( ui->spinBoxScale->value() * 100.0 ) / 100.0 / 100.0;
    open();
}

void PDFAttachmentView::on_spinBoxScale_editingFinished()
{
    m_zoom = ui->spinBoxScale->value() / 100.0;
    open();
}

int PDFAttachmentView::numPages() const
{
    Poppler::Document* doc = attachment<PDFAttachment>()->document();
    if (doc)
    {
        return doc->numPages();
    }
    else
    {
        return 0;
    }
}

void PDFAttachmentView::handlePageControlEnabled()
{
    if (!attachment())
    {
        ui->buttonNextPage->setEnabled( false );
        ui->buttonPreviousPage->setEnabled( false );
        ui->spinBoxPage->setEnabled( false );
    }
    else
    {
        ui->buttonNextPage->setEnabled( true );
        ui->buttonPreviousPage->setEnabled( true );
        ui->spinBoxPage->setEnabled( true );


        int n = numPages();

        if (m_currentPage == n - 1)
        {
            ui->buttonNextPage->setEnabled( false );
        }
        if (m_currentPage == 0 )
        {
            ui->buttonPreviousPage->setEnabled( false );
        }
        ui->spinBoxPage->setRange( 1, n );
        ui->spinBoxPage->setValue( m_currentPage + 1 );
    }
}

void PDFAttachmentView::on_buttonPreviousPage_clicked()
{
    ui->spinBoxPage->setValue( m_currentPage - 1 + 1 );
}

void PDFAttachmentView::on_buttonNextPage_clicked()
{
    ui->spinBoxPage->setValue( m_currentPage + 1 + 1 );
}

void PDFAttachmentView::on_spinBoxPage_valueChanged(int arg1)
{
    PDFAttachment* a = attachment<PDFAttachment>();

    m_currentPage = arg1 - 1;
    assert( m_currentPage >= 0 && m_currentPage < a->document()->numPages() );

    open();
}


QByteArray PDFAttachmentView::options() const
{
    QByteArray o;
    QDataStream stream(&o, QIODevice::WriteOnly);
    stream << m_zoom << m_currentPage;
    return o;
}

void PDFAttachmentView::restoreOptions(const QByteArray &options)
{
    QDataStream stream(options);
    stream >> m_zoom >> m_currentPage;
    handlePageControlEnabled();
    ui->spinBoxScale->setValue( 100 * m_zoom );
}

void PDFAttachmentView::resizeEvent(QResizeEvent *e)
{
    static QTimer* timer = NULL;

    IndexedFileAttachmentView::resizeEvent(e);

    if (timer)
    {
        delete timer;
    }
    timer = new QTimer();
    timer->setSingleShot(true);
    connect( timer, SIGNAL(timeout()), this, SLOT(open()) );
    timer->start( 200 );
}
