#include "pdfattachmentview.h"
#include "ui_pdfattachmentview.h"

DEFN_CREATABLE(PDFAttachmentView, AttachmentView);

PDFAttachmentView::PDFAttachmentView(QWidget *parent) :
    AttachmentView(parent),
    ui(new Ui::PDFAttachmentView)
{
    ui->setupUi(this);
}

PDFAttachmentView::~PDFAttachmentView()
{
    delete ui;
}
