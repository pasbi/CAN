#include "chordpatternattachmentview.h"
#include "ui_chordpatternattachmentview.h"
#include "global.h"
#include <QScrollBar>
#include "util.h"
#include "Database/SongDatabase/song.h"
#include "Database/SongDatabase/songdatabase.h"
#include "project.h"
#include "Commands/AttachmentCommands/ChordPatternAttachmentCommands/chordpatternattachmenttransposecommand.h"

DEFN_CREATABLE(ChordPatternAttachmentView, AttachmentView);
REGISTER_DEFN_CONFIG( ChordPatternAttachmentView, "Chord Pattern Attachment" );

CONFIGURABLE_ADD_ITEM( ChordPatternAttachmentView, WarningColor, QColor(Qt::red), ConfigurationItemOptions::ColorEditorOptions() );

ChordPatternAttachmentView::ChordPatternAttachmentView(QWidget *parent) :
    AttachmentView(parent),
    ui(new Ui::ChordPatternAttachmentView),
    m_minorPolicy( Chord::LowerCase ),
    m_enharmonicPolicy( Chord::Natural )
{
    ui->setupUi(this);
    m_toolBar = new QToolBar( this );


    QAction* transposeUpAction = new QAction( QIcon(":/icons/icons/up51.png"), tr("Transpose up"), this);
    addAction( transposeUpAction );
    connect( transposeUpAction, &QAction::triggered, [this]()
    {
        attachment<Attachment>()->song()->database()->project()->pushCommand(
                    new ChordPatternAttachmentTransposeCommand( attachment<ChordPatternAttachment>(), 1 )
                    );
    });

    QAction* transposeDownAction = new QAction( QIcon(":/icons/icons/down27.png"), tr("Transpose down"), this );
    addAction( transposeDownAction );
    connect( transposeDownAction, &QAction::triggered, [this]()
    {
        attachment<Attachment>()->song()->database()->project()->pushCommand(
                    new ChordPatternAttachmentTransposeCommand( attachment<ChordPatternAttachment>(), -1 )
                    );
    });


    m_toolBar->addActions( actions() );
    ui->verticalLayout->insertWidget(0, m_toolBar);

    ui->textEdit->setFont( QFont("Courier") );
    ui->textEdit->setUndoRedoEnabled(true);

}

ChordPatternAttachmentView::~ChordPatternAttachmentView()
{
    delete ui;
}

void ChordPatternAttachmentView::updateText()
{
    ui->textEdit->blockSignals(true);
    int cursorPosition = ui->textEdit->textCursor().position();
    int scrollbarPosition = ui->textEdit->verticalScrollBar()->value();
    ui->textEdit->setText(attachment<ChordPatternAttachment>()->chordPattern());
    QTextCursor cursor(ui->textEdit->document());
    cursor.setPosition(cursorPosition);
    ui->textEdit->setTextCursor(cursor);
    ui->textEdit->verticalScrollBar()->setValue(scrollbarPosition);

    QList<QTextEdit::ExtraSelection> highlights;
    int i = 0;
    for (QString line : ui->textEdit->toPlainText().split("\n")) {
        QStringList chords, tokens;
        bool isChordLine = Chord::parseLine( line, chords, tokens );
        for ( const QString & token : tokens )
        {
            if (Chord(token).isValid() && isChordLine) {
                QTextCursor cursor(ui->textEdit->document());
                cursor.setPosition(i);
                cursor.setPosition(i + token.length(), QTextCursor::KeepAnchor);
                QTextEdit::ExtraSelection highlight;
                highlight.cursor = cursor;
                highlight.format.setFontUnderline(true);
                highlight.format.setUnderlineColor( QColor(255, 128, 0) );
                highlights << highlight;
            }
            i += token.length() + 1;
        }
    }
    ui->textEdit->setExtraSelections(highlights);
    ui->textEdit->blockSignals(false);
}

void ChordPatternAttachmentView::textEdited()
{
    attachment<ChordPatternAttachment>()->setPattern(ui->textEdit->toPlainText());
    updateText();
}


void ChordPatternAttachmentView::connectWithAttachment()
{
    updateText();
    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(textEdited()));
    connect(attachment<ChordPatternAttachment>(), SIGNAL(changed()), this, SLOT(updateText()));
}
