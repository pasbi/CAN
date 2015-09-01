#include "exportpdfdialog.h"
#include "ui_exportpdfdialog.h"
#include "configurationdialog.h"
#include "Attachments/attachment.h"
#include "Database/SongDatabase/song.h"

DEFN_CONFIG( ExportPDFDialog, "ExportPDFDialog");

CONFIGURABLE_ADD_ITEM_HIDDEN( ExportPDFDialog, include_tags, QStringList() );
CONFIGURABLE_ADD_ITEM_HIDDEN( ExportPDFDialog, exclude_tags, QStringList() );

ExportPDFDialog::ExportPDFDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportPDFDialog)
{
    ui->setupUi(this);

    ui->includeTagsEditor->setTags( config["include_tags"].toStringList() );
    ui->includeTagsEditor->setTags( config["exclude_tags"].toStringList() );
}

ExportPDFDialog::~ExportPDFDialog()
{
    delete ui;
}

bool ExportPDFDialog::test( const Song* song ) const
{
    // test if any attachment matches the criteria
    for (const Attachment* attachment : song->attachments())
    {
        if (test( attachment ))
        {
            return true;
        }
    }
    return false;
}

bool ExportPDFDialog::test( const Attachment *attachment ) const
{

    bool includeOk = config["include_tags"].toStringList().isEmpty();
    for ( const QString& tag : config["include_tags"].toStringList() )
    {
        if (attachment->tags().contains( tag ))
        {
            includeOk = true;
            break;
        }
    }

    if (!includeOk)
    {
        return false;
    }

    for ( const QString& tag : config["exclude_tags"].toStringList() )
    {
        if (attachment->tags().contains( tag ))
        {
            return false;
        }
    }

    return true;
}

void ExportPDFDialog::accept()
{
    config.set("include_tags", ui->includeTagsEditor->tags());
    config.set("exclude_tags", ui->excludeTagsEditor->tags());
    QDialog::accept();
}

void ExportPDFDialog::on_buttonEditPreferences_clicked()
{
    ConfigurationDialog dialog;
    dialog.focusPage( "PDFCreator" );
    dialog.exec();
}
