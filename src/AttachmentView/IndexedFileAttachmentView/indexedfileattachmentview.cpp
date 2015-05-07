#include "indexedfileattachmentview.h"
#include "ui_indexedfileattachmentview.h"
#include "application.h"
#include "Database/SongDatabase/song.h"
#include "Attachments/indexedfileattachment.h"
#include "Commands/AttachmentCommands/fileattachmentcommandsethashcommand.h"
#include "Database/SongDatabase/songdatabase.h"
#include "project.h"
#include "indexedfilechoosedialog.h"

QList<Ui::IndexedFileAttachmentView*> IndexedFileAttachmentView::initializedUis;
QList<Ui::IndexedFileAttachmentView*> IndexedFileAttachmentView::deletedUis;

IndexedFileAttachmentView::IndexedFileAttachmentView(QWidget *parent) :
    AttachmentView(parent),
    ui(new Ui::IndexedFileAttachmentView)
{

    ui->setupUi(this);
    initializedUis << ui;
    connect(ui->buttonChooseFile, SIGNAL(clicked()), this, SLOT(chooseFile()) );
    connect(ui->buttonChooseFileBig, SIGNAL(clicked()), this, SLOT(chooseFile()) );
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
//    QStringList endings = attachment<IndexedFileAttachment>()->acceptedEndings();

//    ui->advancedFileChooser->setFilterProperties( attachment()->song(), endings );

//    connect( ui->advancedFileChooser, &AdvancedFileChooser::itemSelected, [this](QByteArray hash)
//    {
//        Command* c = new FileAttachmentCommandSetHashCommand( attachment<IndexedFileAttachment>(), hash );
//        app().pushCommand( c );
//    });

//    ui->advancedFileChooser->blockSignals(true);    // do not create a command (see connect above)
//    ui->advancedFileChooser->setHash( attachment<IndexedFileAttachment>()->hash() );
//    open();
//    attachment<IndexedFileAttachment>()->open();
//    ui->advancedFileChooser->blockSignals(false);

    connect( attachment<IndexedFileAttachment>(), &IndexedFileAttachment::hashChanged, [this]()
    {
        open();
        updateStackedWidget();
    });
    updateStackedWidget();
}

void IndexedFileAttachmentView::updateAttachmentView()
{
//    ui->advancedFileChooser->updateComboBox();
}

void IndexedFileAttachmentView::updateStackedWidget()
{
    IndexedFileAttachment* a = attachment<IndexedFileAttachment>();

    if (a && a->fileExists())
    {
        ui->stackedWidget->setCurrentIndex(1);
        ui->labelFilename->setText( QFileInfo(a->filename()).fileName() );
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(0);
        ui->labelFilename->setText( tr("< No File >") );
    }
}

void IndexedFileAttachmentView::chooseFile()
{
    QStringList endings = attachment<IndexedFileAttachment>()->acceptedEndings();
    const IndexedFileAttachment* ifa = attachment<IndexedFileAttachment>();
    IndexedFileChooseDialog dialog( ifa->song(), ifa->hash(), endings );

    if (dialog.exec() == QDialog::Accepted)
    {
        app().pushCommand( new FileAttachmentCommandSetHashCommand( attachment<IndexedFileAttachment>(), dialog.hash() ) );
    }
}