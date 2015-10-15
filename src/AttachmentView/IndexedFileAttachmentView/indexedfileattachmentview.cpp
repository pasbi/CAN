#include "indexedfileattachmentview.h"
#include "ui_indexedfileattachmentview.h"
#include "application.h"
#include "Database/SongDatabase/song.h"
#include "Attachments/indexedfileattachment.h"
#include "Commands/AttachmentCommands/fileattachmentcommandsethashcommand.h"
#include "Database/SongDatabase/songdatabase.h"
#include "Project/project.h"
#include "Dialogs/IndexedFileChooseDialog/indexedfilechoosedialog.h"
#include <QFileInfo>


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

    if (a && !a->filename().isEmpty())
    {
        ui->stackedWidget->setCurrentIndex(1);
        ui->labelFilename->setText( QFileInfo(a->filename()).fileName() );
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(0);
        if (!a || a->hash().isEmpty())
        {
            ui->labelFilename->setText( tr("< No File >") );
        }
        else
        {
            ui->labelFilename->setText( tr("There is a file set, but it is unaccessible.\n"
                                           "Think twice before you replace it.\n"
                                           "%1").arg(QString(a->hash().toHex())));
        }
    }
}

void IndexedFileAttachmentView::chooseFile()
{
    IndexedFileAttachment* ifa = attachment<IndexedFileAttachment>();
    IndexedFileChooseDialog dialog( ifa->song(), ifa->hash(), ifa->acceptedEndings() );

    if (dialog.exec() == QDialog::Accepted)
    {
        app().pushCommand( new FileAttachmentCommandSetHashCommand( ifa, dialog.hash() ) );
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
