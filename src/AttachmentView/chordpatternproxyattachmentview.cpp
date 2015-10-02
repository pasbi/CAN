#include "chordpatternproxyattachmentview.h"
#include "ui_chordpatternproxyattachmentview.h"
#include "Dialogs/chordpatternviewer.h"
#include "application.h"
#include "Attachments/ChordPatternAttachment/chordpatternproxyattachment.h"
#include "Attachments/ChordPatternAttachment/chordpatternattachment.h"
#include "chordpatternattachmentview.h"
#include "Database/SongDatabase/song.h"
#include "Commands/AttachmentCommands/abstractchordpatternattachmenttransposecommand.h"

DEFN_CREATABLE(ChordPatternProxyAttachmentView, AttachmentView);

ChordPatternProxyAttachmentView::ChordPatternProxyAttachmentView(QWidget *parent) :
    AttachmentView(parent),
    ui(new Ui::ChordPatternProxyAttachmentView)
{
    ui->setupUi(this);
}

ChordPatternProxyAttachmentView::~ChordPatternProxyAttachmentView()
{
    delete ui;
}

void ChordPatternProxyAttachmentView::polish()
{
    attachment<ChordPatternProxyAttachment>()->updateCache();
    connect(attachment<ChordPatternProxyAttachment>()->source(), SIGNAL(changed()), this, SLOT(updateText()));
    updateText();
    updateViewIcon();
}

void ChordPatternProxyAttachmentView::on_buttonUp_clicked()
{
    app().pushCommand( new AbstractChordPatternAttachmentTransposeCommand( attachment<ChordPatternProxyAttachment>(),  1 ) );
    if (attachment<ChordPatternProxyAttachment>())
    {
        attachment<ChordPatternProxyAttachment>()->updateCache();
        updateText();
    }
}

void ChordPatternProxyAttachmentView::on_buttonDown_clicked()
{
    app().pushCommand( new AbstractChordPatternAttachmentTransposeCommand( attachment<ChordPatternProxyAttachment>(), -1 ) );
    if (attachment<ChordPatternProxyAttachment>())
    {
        attachment<ChordPatternProxyAttachment>()->updateCache();
        updateText();
    }
}

void ChordPatternProxyAttachmentView::on_buttonView_clicked()
{
    ChordPatternViewer::displayChordPatternAttachment( attachment<AbstractChordPatternAttachment>(), this );
}

void ChordPatternProxyAttachmentView::updateText()
{
    ChordPatternProxyAttachment* a = attachment<ChordPatternProxyAttachment>();
    const ChordPatternAttachment* source = a ? a->source() : nullptr;
    if (a)
    {
        ChordPatternAttachmentView::highlightTextEdit( ui->textEdit, a->chordPattern() );
    }
    else
    {
        ui->textEdit->clear();
    }

    if (source)
    {
        int t = attachment<ChordPatternProxyAttachment>()->transpose();
        if (t > 6)
        {
            t -= 12;
        }
        QString transpose = QString("%1").arg(t);
        if (t > 0)
        {
            transpose = "+" + transpose;
        }
        else if (t == 0)
        {
            transpose = QChar(0x00B1) + transpose;
        }
        ui->buttonOriginal->setText( "< " + source->name() + " > [" + transpose + "]");
        ui->buttonOriginal->setEnabled( true );
    }
    else
    {
        ui->buttonOriginal->setText( "" );
        ui->buttonOriginal->setEnabled( false );
    }
}

void ChordPatternProxyAttachmentView::on_buttonOriginal_clicked()
{
    emit focusAttachment( attachment<ChordPatternProxyAttachment>()->source() );
}

void ChordPatternProxyAttachmentView::updateViewIcon()
{
    if (attachment<Attachment>()->song()->program().isValid())
    {
        ui->buttonView->setIcon( QIcon(":/icons/icons/eye_midi.png") );
    }
}
