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

    connect( ui->advancedFileChooser, &AdvancedFileChooser::itemSelected, [this](QByteArray hash) {
        attachment<IndexedFileAttachment>()->setHash(hash);
        updateStackedWidget();
    });
}

IndexedFileAttachmentView::~IndexedFileAttachmentView()
{
    delete ui;
}

void IndexedFileAttachmentView::setWidget(QWidget *widget)
{
    assert( ui->stackedWidget->count() == 1 );
    ui->stackedWidget->addWidget(widget);

    updateStackedWidget();
}

void IndexedFileAttachmentView::polish()
{
    QStringList endings = attachment<IndexedFileAttachment>()->acceptedEndings();

    ui->advancedFileChooser->setFilterProperties( attachment()->song(), endings );

    connect( ui->advancedFileChooser, SIGNAL(itemSelected(QByteArray)), attachment<IndexedFileAttachment>(), SLOT(setHash(QByteArray)));
    ui->advancedFileChooser->setHash( attachment<IndexedFileAttachment>()->hash() );
    connect( attachment<IndexedFileAttachment>(), SIGNAL(hashChanged(QByteArray)), this, SLOT(open()) );
}

void IndexedFileAttachmentView::updateAttachmentView()
{
    ui->advancedFileChooser->updateComboBox();
}

void IndexedFileAttachmentView::updateStackedWidget()
{
    qDebug() << "update SW";
    IndexedFileAttachment* a = attachment<IndexedFileAttachment>();

    if (a && a->fileExists())
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
}
