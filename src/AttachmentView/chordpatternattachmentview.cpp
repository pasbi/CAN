#include "chordpatternattachmentview.h"
#include "ui_chordpatternattachmentview.h"
#include "global.h"
#include "Attachments/ChordPatternAttachment/chordpatternattachment.h"
#include <QScrollBar>
#include "util.h"

DEFN_CREATABLE(ChordPatternAttachmentView, AttachmentView);

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

    QAction* transposeDownAction = new QAction( QIcon(":/icons/icons/down27.png"), tr("Transpose down"), this );
    addAction( transposeDownAction );

    QAction* recognizeChordsAction = new QAction( QIcon(":/icons/icons/three91.png"), tr("Recognize Chords"), this);
    addAction( recognizeChordsAction );
    connect( recognizeChordsAction, SIGNAL(triggered()), this, SLOT(updateText()) );

    m_toolBar->addActions( actions() );
    ui->verticalLayout->insertWidget(0, m_toolBar);

    ui->textEdit->setFont( QFont("Courier") );
}

ChordPatternAttachmentView::~ChordPatternAttachmentView()
{
    delete ui;
}

void setPositionIfValid( QTextCursor& cursor, int pos, QTextCursor::MoveMode  mode)
{
    if ( pos >= 0 && pos < cursor.document()->characterCount() )
    {
        cursor.setPosition( pos, mode );
    }
}

void ChordPatternAttachmentView::updateText()
{
    ChordPatternAttachment* patternAttachment = attachment<ChordPatternAttachment>();
    if (!patternAttachment)
    {
        ui->textEdit->clear();
    }
    else
    {
        int cursorPosition = ui->textEdit->textCursor().position();
        int scrollbarPosition = ui->textEdit->verticalScrollBar()->value();

        patternAttachment->chordPattern().parse( ui->textEdit->toPlainText() );
        ui->textEdit->setText(patternAttachment->text( m_minorPolicy, m_enharmonicPolicy ));

        QTextCursor cursor(ui->textEdit->document());
        cursor.setPosition(cursorPosition);
        ui->textEdit->setTextCursor(cursor);
        ui->textEdit->verticalScrollBar()->setValue(scrollbarPosition);

        QList<QTextEdit::ExtraSelection> highlights;
        int row = 0;
        int offset = 0;
        for (const Line* line : patternAttachment->chordPattern().lines() )
        {
            if (line->type() == Line::Chords)
            {
                for (const Chord* c : line->chords())
                {
                    int position = c->column() + offset;
                    QTextCursor cursor(ui->textEdit->document());
                    setPositionIfValid( cursor, position, QTextCursor::MoveAnchor );
                    setPositionIfValid( cursor, position + c->toString( m_minorPolicy, m_enharmonicPolicy ).length(), QTextCursor::KeepAnchor);
                    QTextEdit::ExtraSelection highlight;
                    highlight.cursor = cursor;
                    highlight.format.setFontUnderline(true);
                    highlight.format.setUnderlineColor( QColor(255, 128, 0) );
                    highlights << highlight;
                }
            }
            else
            {
                // no highlights
            }
            row++;
            offset += line->length( m_minorPolicy, m_enharmonicPolicy ) + 1;    // dont forget the linebreak
        }
        ui->textEdit->setExtraSelections(highlights);
    }
}
