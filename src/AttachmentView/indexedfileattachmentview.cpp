#include "indexedfileattachmentview.h"
#include "ui_indexedfileattachmentview.h"
#include "application.h"
#include "Database/SongDatabase/song.h"
#include "Attachments/indexedfileattachment.h"
#include "Commands/AttachmentCommands/fileattachmentcommandsethashcommand.h"
#include "Database/SongDatabase/songdatabase.h"
#include "project.h"


QList<Ui::IndexedFileAttachmentView*> IndexedFileAttachmentView::initializedUis;
QList<Ui::IndexedFileAttachmentView*> IndexedFileAttachmentView::deletedUis;

IndexedFileAttachmentView::IndexedFileAttachmentView(QWidget *parent) :
    AttachmentView(parent),
    ui(new Ui::IndexedFileAttachmentView)
{

    ui->setupUi(this);
    initializedUis << ui;

    connect( ui->advancedFileChooser, &AdvancedFileChooser::itemSelected, [this]() {
        updateStackedWidget();
    });

}

IndexedFileAttachmentView::~IndexedFileAttachmentView()
{
    deletedUis << ui;
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

    connect( ui->advancedFileChooser, &AdvancedFileChooser::itemSelected, [this](QByteArray hash)
    {
        Command* c = new FileAttachmentCommandSetHashCommand( attachment<IndexedFileAttachment>(), hash );
        attachment<Attachment>()->song()->database()->project()->pushCommand( c );
    });

    ui->advancedFileChooser->blockSignals(true);    // do not create a command (see connect above)
    ui->advancedFileChooser->setHash( attachment<IndexedFileAttachment>()->hash() );
    open();
    attachment<IndexedFileAttachment>()->open();
    updateStackedWidget();
    ui->advancedFileChooser->blockSignals(false);

    connect( attachment<IndexedFileAttachment>(), SIGNAL(hashChanged(QByteArray)), this, SLOT(open()) );
    connect( attachment<IndexedFileAttachment>(), &IndexedFileAttachment::hashChanged, [this](QByteArray hash)
    {
        ui->advancedFileChooser->blockSignals(true);
        ui->advancedFileChooser->setHash( hash );
        ui->advancedFileChooser->blockSignals(false);
    });
}

void IndexedFileAttachmentView::updateAttachmentView()
{
    ui->advancedFileChooser->updateComboBox();
}

void IndexedFileAttachmentView::updateStackedWidget()
{
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
