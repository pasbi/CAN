#include "chordpatternattachmentview.h"
#include "ui_chordpatternattachmentview.h"
#include "global.h"
#include "Attachments/ChordPatternAttachment/chordpatternattachment.h"
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

void ChordPatternAttachmentView::savePositions( int& cursorPosition, int& scrollbarPosition )
{
    cursorPosition = ui->textEdit->textCursor().position();
    scrollbarPosition = ui->textEdit->verticalScrollBar()->value();
}

void ChordPatternAttachmentView::readText()
{
    ChordPatternAttachment* patternAttachment = attachment<ChordPatternAttachment>();
    if (patternAttachment)
    {
        patternAttachment->chordPattern()->parse( ui->textEdit->toPlainText() );
    }
}


void ChordPatternAttachmentView::writeText(int cursorPosition, int scrollbarPosition)
{
    ChordPatternAttachment* patternAttachment = attachment<ChordPatternAttachment>();

    if (!patternAttachment)
    {
        ui->textEdit->clear();
    }
    else
    {
        ui->textEdit->setText(patternAttachment->text( m_minorPolicy, m_enharmonicPolicy ));

        QTextCursor cursor(ui->textEdit->document());
        cursor.setPosition(cursorPosition);
        ui->textEdit->setTextCursor(cursor);
        ui->textEdit->verticalScrollBar()->setValue(scrollbarPosition);


        QList<QTextEdit::ExtraSelection> highlights;
        int row = 0;
        int offset = 0;
        for (const Line* line : patternAttachment->chordPattern()->lines() )
        {
            int transpose = attachment<ChordPatternAttachment>()->chordPattern()->transpose();
            if (line->type() == Line::Chords)
            {
                for (const Chord* c : line->chords())
                {
                    int position = c->column() + offset;
                    QTextCursor cursor(ui->textEdit->document());
                    setPositionIfValid( cursor, position, QTextCursor::MoveAnchor );
                    int chordLength = c->toString( transpose, m_minorPolicy, m_enharmonicPolicy ).length();
                    setPositionIfValid( cursor, position + chordLength, QTextCursor::KeepAnchor);
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
            offset += line->length( transpose, m_minorPolicy, m_enharmonicPolicy ) + 1;    // dont forget the linebreak
        }
        ui->textEdit->setExtraSelections(highlights);
    }


}

void ChordPatternAttachmentView::updateText()
{
    int cursorPosition, scrollbarPosition;
    savePositions( cursorPosition, scrollbarPosition );
    readText( );
    writeText( cursorPosition, scrollbarPosition );

    removeWarningSign();
}


void ChordPatternAttachmentView::connectWithAttachment()
{
    connect( attachment<ChordPatternAttachment>(), &ChordPatternAttachment::changed, [this](){

        int oldNumChords = ChordPattern::countChords( ui->textEdit->toPlainText() );
        int cursorPosition, scrollbarPosition;
        savePositions( cursorPosition, scrollbarPosition );
        writeText( cursorPosition, scrollbarPosition );

        int newNumChords = ChordPattern::countChords( ui->textEdit->toPlainText() );
        if (oldNumChords > newNumChords )
        {
            putWarningSign();
        }
        else if ( newNumChords > oldNumChords )
        {
            removeWarningSign();
        }
    });
}

void ChordPatternAttachmentView::putWarningSign()
{
    //Configurable::item("WarningColor").value<QColor>()
    ui->textEdit->setTextBackgroundColor( QColor(Qt::red).darker(300) );
}

void ChordPatternAttachmentView::removeWarningSign()
{
    ui->textEdit->setTextBackgroundColor( QColor( 0, 0, 0, 0 ) );
}
