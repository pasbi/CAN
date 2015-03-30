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
            setAttachment( index );
        }
        else
        {
            setAttachment( -1 );
        }
    });
}

AttachmentChooser::~AttachmentChooser()
{
    delete ui;
}

void AttachmentChooser::setSong(Song *song)
{
    // remeber index

    if (m_song)
    {
        m_lastOpenedIndex[m_song] = ui->comboBox->currentIndex();
    }


    m_song = song;
    ui->comboBox->clear();
    if (song == NULL)
    {
        setAttachment( -1 );
    }
    else
    {
        ui->comboBox->addItems( song->attachmentNames() );

        // restore last opened index
        ui->comboBox->setCurrentIndex( m_lastOpenedIndex.value( m_song, 0 ) );
    }
}

void AttachmentChooser::setAttachment( int index )
{
    ui->comboBox->clear();
    if (!m_song || index < 0)
    {
        m_currentAttachment = NULL;
        ui->attachmentEditor->setAttachment( NULL );
    }
    else
    {
        ui->comboBox->blockSignals(true);
        ui->comboBox->clear();
        ui->comboBox->addItems( m_song->attachmentNames() );

        index = qBound( 0, index, song()->attachments().length() - 1);

        if (index < 0 || index >= song()->attachments().length())
        {
            ui->comboBox->setCurrentIndex( -1 );
            m_currentAttachment = NULL;
            ui->attachmentEditor->setAttachment( NULL );
        }
        else
        {
            ui->comboBox->setCurrentIndex( index );
            m_currentAttachment = m_song->attachment(index);
            ui->attachmentEditor->setAttachment( m_currentAttachment );
        }
        ui->comboBox->blockSignals(false);
    }
}

int AttachmentChooser::currentAttachmentIndex() const
{
    return ui->comboBox->currentIndex();
}

Attachment* AttachmentChooser::currentAttachment() const
{
    return m_currentAttachment;
}

void AttachmentChooser::updateAttachmentView()
{
    ui->attachmentEditor->updateAttachmentView();
}
