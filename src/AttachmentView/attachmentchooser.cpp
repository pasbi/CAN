#include "attachmentchooser.h"
#include "ui_attachmentchooser.h"

AttachmentChooser::AttachmentChooser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AttachmentChooser)
{
    ui->setupUi(this);
    connect( ui->comboBox, static_cast< void (QComboBox::*)(int) >(&QComboBox::currentIndexChanged), [this](int index)
    {
        if (m_song && index >= 0)
        {
            update( m_song->attachments()[index] );
        }
        else
        {
            update( NULL );
        }
    });
}

AttachmentChooser::~AttachmentChooser()
{
    delete ui;
}

void AttachmentChooser::setSong(Song *song)
{
    m_song = song;
    if (song == NULL)
    {
        update( NULL );
    }
    else
    {
        if (song->attachments().isEmpty())
        {
            update( NULL );
        }
        else
        {
            update( song->attachments().first() );
        }
    }

    if (song == NULL)
    {
        ui->comboBox->clear();
        ui->attachmentEditor->setAttachment( NULL );
    }
    else
    {
        ui->comboBox->clear();
        ui->comboBox->addItems( song->attachmentNames() );
        if (song->attachments().isEmpty())
        {
            ui->attachmentEditor->setAttachment( NULL );
        }
        else
        {
            ui->attachmentEditor->setAttachment( song->attachments().first() );
        }
    }
}

void AttachmentChooser::update( Attachment* attachment )
{

    if (attachment == NULL)
    {
        ui->comboBox->clear();
        ui->attachmentEditor->setAttachment( NULL );
    }
    else
    {
        assert( attachment->song() == m_song );
        ui->comboBox->blockSignals(true);
        ui->comboBox->clear();
        ui->comboBox->addItems( attachment->song()->attachmentNames() );
        if ( song()->attachments().isEmpty() )
        {
            ui->attachmentEditor->setAttachment( NULL );
            ui->comboBox->setCurrentIndex( -1 );
        }
        else
        {
            ui->attachmentEditor->setAttachment( attachment );
            ui->comboBox->setCurrentIndex( m_song->attachments().indexOf(attachment) );
        }
        ui->comboBox->blockSignals(false);
    }
}
