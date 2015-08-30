#include "indexedfileattachmentview.h"
#include "ui_indexedfileattachmentview.h"
#include "application.h"
#include "Database/SongDatabase/song.h"
#include "Attachments/indexedfileattachment.h"
#include "Commands/AttachmentCommands/fileattachmentcommandsethashcommand.h"
#include "Database/SongDatabase/songdatabase.h"
#include "Project/project.h"
#include "indexedfilechoosedialog.h"


IndexedFileAttachmentView::IndexedFileAttachmentView(QWidget *parent) :
    AttachmentView(parent),
    ui(new Ui::IndexedFileAttachmentView)
{

    ui->setupUi(this);
    connect(ui->buttonChooseFile, SIGNAL(clicked()), this, SLOT(chooseFile()) );
    connect(ui->buttonChooseFileBig, SIGNAL(clicked()), this, SLOT(chooseFile()) );
}

IndexedFileAttachmentView::~IndexedFileAttachmentView()
{
    delete ui;
}

void IndexedFileAttachmentView::setWidget(QWidget *widget)
{
    assert( ui->stackedWidget->count() == 1 );
    ui->stackedWidget->addWidget(widget);
}

void IndexedFileAttachmentView::polish()
{
    connect( attachment<IndexedFileAttachment>(), &IndexedFileAttachment::hashChanged, [this]()
    {
        updateStackedWidget();
        open();
    });
    updateStackedWidget();
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

void IndexedFileAttachmentView::open()
{
    IndexedFileAttachment* ifa = attachment<IndexedFileAttachment>();
    if (ifa)
    {
       ifa->open();
    }
}
