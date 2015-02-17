#include "chordpatternedit.h"
#include "global.h"
#include <QKeyEvent>

ChordPatternEdit::ChordPatternEdit(QWidget *parent) :
    QTextEdit(parent)
{
    /////////////
    /// Currently no support for undo/redo in ChordPatternAttachment since it seems to be very
    /// hard to connect the documents undo stack to the application wide undo stack.
    setFont( QFont("Courier") );
    setUndoRedoEnabled(true);
}

