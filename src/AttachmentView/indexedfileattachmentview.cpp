#include "indexedfileattachmentview.h"
#include "ui_indexedfileattachmentview.h"
#include "application.h"
#include "Database/SongDatabase/song.h"
#include "Attachments/indexedfileattachment.h"

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

void IndexedFileAttachmentView::polish()
{
    QStringList endings = attachment<IndexedFileAttachment>()->acceptedEndings();

    ui->widget->setFilterProperties( attachment()->song(), endings );

    connect( ui->widget, SIGNAL(itemSelected(QByteArray)), attachment<IndexedFileAttachment>(), SLOT(setHash(QByteArray)));
    ui->widget->setHash( attachment<IndexedFileAttachment>()->hash() );
}

void IndexedFileAttachmentView::updateAttachmentView()
{
    ui->widget->updateComboBox();
}
