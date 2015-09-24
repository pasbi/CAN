#include "attachmentchooser.h"

#include <QLineEdit>
#include <QKeyEvent>

#include "ui_attachmentchooser.h"
#include "Dialogs/tagdialog.h"
#include "Commands/edittagscommand.h"
#include "application.h"
#include "mainwindow.h"
#include "Commands/AttachmentCommands/attachmentrenamecommand.h"
#include "Commands/SongCommands/songremoveattachmentcommand.h"
#include "Attachments/attachment.h"
#include "Database/SongDatabase/song.h"

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
        app().mainWindow()->updateActionsEnabled();
    });
    ui->comboBox->setInvalidText( tr("No Attachment") );
    ui->comboBox->installEventFilter(this);
    ui->comboBox->setInsertPolicy(QComboBox::NoInsert);

    m_editTagAction = new QAction( this );
    m_editTagAction->setIcon( QIcon(":/icons/icons/tag-2.png") );
    connect( m_editTagAction, SIGNAL(triggered()), this, SLOT(editTags()) );
    m_editTagAction->setText( tr("Edit tags ...") );

    ui->toolButton->addAction( m_editTagAction );
    ui->toolButton->addAction( app().mainWindow()->newAttachment_Action( "ChordPatternAttachment" ) );
    ui->toolButton->addAction( app().mainWindow()->newAttachment_Action( "ChordPatternProxyAttachment" ) );
    ui->toolButton->addAction( app().mainWindow()->newAttachment_Action( "AudioAttachment" ) );
    ui->toolButton->addAction( app().mainWindow()->newAttachment_Action( "PDFAttachment" ) );

    connect( ui->attachmentEditor, SIGNAL(focusAttachment(const Attachment*)), this, SLOT(focusAttachment(const Attachment*)) );

    setSong( NULL );
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

    ui->toolButton->setEnabled( !!song );
    // other actions in toolButton are updated in mainwindow

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
    m_editTagAction->setEnabled( song() && currentAttachment() );

    ui->buttonDelete->setEnabled( !!m_currentAttachment );
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

void AttachmentChooser::editTags()
{
    if (currentAttachment())
    {
        TagDialog dialog( currentAttachment()->tags(), this );
        if (dialog.exec() == QDialog::Accepted)
        {
            app().pushCommand( new EditTagsCommand(currentAttachment(), dialog.tags() ));
        }
    }
}

void AttachmentChooser::focusAttachment(const Attachment *a)
{
    if (song())
    {
        int index = -1;
        for (int i = 0; i < song()->attachments().length(); ++i)
        {
            if (song()->attachment(i) == a)
            {
                index = i;
                break;
            }
        }
        if (index < -1)
        {
            qWarning() << "song " << song() << " has no attachment " << a << ".";
        }
        else
        {
            setAttachment( index );
        }
    }

    app().mainWindow()->updateActionsEnabled();
}

bool AttachmentChooser::eventFilter(QObject *o, QEvent *e)
{
    // catch focus out and enter/escape key press events and make editable combobox non-editable again.
    Attachment* attachment = currentAttachment();
    if (o == ui->comboBox && ui->comboBox->lineEdit() && attachment)
    {
        QString newText = ui->comboBox->lineEdit()->text();
        switch (e->type())
        {
        case QEvent::FocusOut:
            app().pushCommand( new AttachmentRenameCommand(attachment, newText));
            ui->comboBox->setEditable(false);
            break;
        case QEvent::KeyPress:  // we need to catch the enter-press event since insert-policy is set to NoInsert
            switch (static_cast<QKeyEvent*>(e)->key())
            {
            case Qt::Key_Return:
            case Qt::Key_Enter:
                app().pushCommand( new AttachmentRenameCommand(attachment, newText));
            case Qt::Key_Escape:
                ui->comboBox->setEditable(false);
            default:
                ;
            }
        default:
            ;
        }
    }
    return QWidget::eventFilter(o, e);
}

void AttachmentChooser::renameCurrentAttachment()
{
    Attachment* attachment = currentAttachment();
    if (attachment)
    {
        ui->comboBox->setEditable(true);
        ui->comboBox->lineEdit()->selectAll();
        ui->comboBox->setFocus();
    }
}

void AttachmentChooser::on_buttonDelete_clicked()
{
    Attachment* attachment = currentAttachment();
    if (attachment)
    {
        app().pushCommand( new SongRemoveAttachmentCommand(attachment->song(), attachment->song()->attachments().indexOf(attachment) ) );
    }
}
