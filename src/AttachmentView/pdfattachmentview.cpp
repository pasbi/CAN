#include "pdfattachmentview.h"
#include "ui_pdfattachmentview.h"

#include "global.h"

DEFN_CREATABLE(PDFAttachmentView, AttachmentView);

PDFAttachmentView::PDFAttachmentView(QWidget *parent) :
    IndexedFileAttachmentView(parent),
    ui(new Ui::PDFAttachmentView)
{
    m_pdfWidget = new QWidget(this);
    ui->setupUi(m_pdfWidget);
    setWidget( m_pdfWidget );
}

PDFAttachmentView::~PDFAttachmentView()
{
    delete ui;
}

void PDFAttachmentView::connectWithAttachment()
{
}
