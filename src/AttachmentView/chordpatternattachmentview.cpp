#include "chordpatternattachmentview.h"
#include "ui_chordpatternattachmentview.h"
#include "global.h"
#include <QScrollBar>
#include "util.h"
#include "Database/SongDatabase/song.h"
#include "Database/SongDatabase/songdatabase.h"
#include "Project/project.h"
#include "Commands/AttachmentCommands/abstractchordpatternattachmenttransposecommand.h"
#include "Dialogs/chordpatternviewer.h"
#include "Attachments/ChordPatternAttachment/chordpatternattachment.h"

DEFN_CREATABLE(ChordPatternAttachmentView, AttachmentView);

ChordPatternAttachmentView::ChordPatternAttachmentView(QWidget *parent) :
    AttachmentView(parent),
    ui(new Ui::ChordPatternAttachmentView)
{
    ui->setupUi(this);
}

ChordPatternAttachmentView::~ChordPatternAttachmentView()
{
    delete ui;
}


void ChordPatternAttachmentView::updateText()
{
    ui->textEdit->setChordPattern( attachment<ChordPatternAttachment>()->chordPattern() );
}

//#include "Commands/AttachmentCommands/chordpatternattachmenteditpatterncommand.h"
//void ChordPatternAttachmentView::textEdited()
//{
//    pushCommand( new ChordPatternAttachmentEditPatternCommand( attachment<ChordPatternAttachment>(), ui->textEdit->toPlainText() ) );
//}

void ChordPatternAttachmentView::polish()
{
    updateText();
//    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(textEdited()));
    connect(attachment<ChordPatternAttachment>(), SIGNAL(changed()), this, SLOT(updateText()));
    connect( ui->textEdit, SIGNAL(pasted()), attachment<ChordPatternAttachment>(), SLOT(transpose()) );
    updateViewIcon();
}

void ChordPatternAttachmentView::on_buttonUp_clicked()
{
    pushCommand( new AbstractChordPatternAttachmentTransposeCommand( attachment<ChordPatternAttachment>(), 1 ) );
}

void ChordPatternAttachmentView::on_buttonDown_clicked()
{
    pushCommand( new AbstractChordPatternAttachmentTransposeCommand( attachment<ChordPatternAttachment>(), -1 ) );
}

void ChordPatternAttachmentView::on_buttonView_clicked()
{
    ChordPatternViewer::displayChordPatternAttachment( attachment<AbstractChordPatternAttachment>(), this );
}

void ChordPatternAttachmentView::updateViewIcon()
{
    if (attachment<Attachment>()->song()->program().isValid())
    {
        ui->buttonView->setIcon( QIcon(":/icons/icons/eye_midi.png") );
    }
}
