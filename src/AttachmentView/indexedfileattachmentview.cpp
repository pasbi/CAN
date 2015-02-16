#include "indexedfileattachmentview.h"
#include "ui_indexedfileattachmentview.h"

IndexedFileAttachmentView::IndexedFileAttachmentView(QWidget *parent) :
    AttachmentView(parent),
    ui(new Ui::IndexedFileAttachmentView)
{
    ui->setupUi(this);
}

IndexedFileAttachmentView::~IndexedFileAttachmentView()
{
    delete ui;
}

void IndexedFileAttachmentView::setWidget(QWidget *widget)
{
    ui->stackedWidget->addWidget(widget);
}
