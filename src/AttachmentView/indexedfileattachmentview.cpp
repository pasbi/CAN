#include "indexedfileattachmentview.h"
#include "ui_indexedfileattachmentview.h"
#include "application.h"

IndexedFileAttachmentView::IndexedFileAttachmentView(QWidget *parent) :
    AttachmentView(parent),
    ui(new Ui::IndexedFileAttachmentView)
{
    ui->setupUi(this);
    ui->widget->setIndex( &app().fileIndex() );
}

IndexedFileAttachmentView::~IndexedFileAttachmentView()
{
    delete ui;
}

void IndexedFileAttachmentView::setWidget(QWidget *widget)
{
    ui->stackedWidget->addWidget(widget);
}
