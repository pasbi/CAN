#include "exportpdfdialog.h"
#include "ui_exportpdfdialog.h"
#include "preferencedialog.h"
#include "Attachments/attachment.h"
#include "Database/SongDatabase/song.h"
#include "application.h"


ExportPDFDialog::ExportPDFDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportPDFDialog)
{
    ui->setupUi(this);

    ui->includeTagsEditor->setTags( app().preference<QStringList>("include_tags") );
    ui->includeTagsEditor->setTags( app().preference<QStringList>("exclude_tags") );
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

    bool includeOk = app().preference<QStringList>("include_tags").isEmpty();
    for ( const QString& tag : app().preference<QStringList>("include_tags") )
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

    for ( const QString& tag : app().preference<QStringList>("exclude_tags"))
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
    app().setPreference( "include_tags", ui->includeTagsEditor->tags());
    app().setPreference( "exclude_tags", ui->excludeTagsEditor->tags());
    QDialog::accept();
}

void ExportPDFDialog::on_buttonEditPreferences_clicked()
{
    //TODO
//    ConfigurationDialog dialog;
//    dialog.focusPage( "PDFCreator" );
//    dialog.exec();
}
