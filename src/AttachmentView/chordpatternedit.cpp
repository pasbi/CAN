#include "chordpatternedit.h"
#include "global.h"
#include <QKeyEvent>
#include <QAction>
#include "application.h"
#include <QMenu>
#include <QDesktopWidget>
#include <QRect>
#include <QMimeData>

ChordPatternEdit::ChordPatternEdit(QWidget *parent) :
    QTextEdit(parent)
{
    setFont( QFont("Courier") );
    setUndoRedoEnabled(false);

    setContextMenuPolicy( Qt::DefaultContextMenu );

    installEventFilter( this );
}


void ChordPatternEdit::contextMenuEvent(QContextMenuEvent *e)
{
    QMenu* menu = createStandardContextMenu( e->pos() );

    int width = menu->sizeHint().width();
    int height = menu->sizeHint().height();

    QPoint pos = e->globalPos();
    QRect desktop = QApplication::desktop()->rect();
    if ( desktop.width() - e->globalX() < width )
    {
        pos.setX( desktop.width() - width );
    }

    if ( desktop.height() - e->globalY() < height )
    {
        pos.setY( pos.y() - height );
    }

    menu->removeAction( menu->actions()[0] );   // remove undo
    menu->removeAction( menu->actions()[0] );   // remove redo
    menu->show();
    menu->move( pos );
}

void ChordPatternEdit::keyPressEvent(QKeyEvent* e)
{
    // catch undo and redo shortcuts and delegate to application's undo()/redo().
    // Does not work with QShortcutEvent or QActions.
    QKeyEvent* se = dynamic_assert_cast<QKeyEvent*>( e );
    if ( se->modifiers() & Qt::CTRL )
    {
        if ( se->key() == Qt::Key_Z)
        {
            app().undo();
            se->accept();
            return;
        }
        else if ( se->key() == Qt::Key_Y )
        {
            app().redo();
            se->accept();
            return;
        }
    }

    // catch tab key. Tabs should be replaced by whitespaces.
    // Actually this is done in ChordPatternAttachment::setPattern,
    // but this will not affect the cursor position.
    // To provide a natural editing experience, replace tabs here and
    // affect cursor position.
    if ( se->key() == Qt::Key_Tab )
    {
        // find position in row
        QString text = toPlainText();
        int pos = textCursor().position();
        int i = 0;
        for (int j = 0; j < pos; ++j)
        {
            i++;
            if (text[j] == '\n')
            {
                i = 0;
            }
        }

        // compute length of tab in whitespaces
        int tabWidth = 8;
        int numberOfWhitespaces = tabWidth - (i % tabWidth);

        // build replacement string
        QString whiteSpaces;
        for (int i = 0; i < numberOfWhitespaces; ++i)
        {
            whiteSpaces += " ";
        }

        // replace tab with whitespaces
        setPlainText( text.insert(pos, whiteSpaces) );

        // update cursor position
        QTextCursor cursor = textCursor();
        cursor.setPosition( pos + numberOfWhitespaces );
        setTextCursor( cursor );

        // do not insert actual tab.
        e->accept();
        return;
    }

    return QTextEdit::keyPressEvent(e);
}

void ChordPatternEdit::insertFromMimeData(const QMimeData *source)
{
    QTextEdit::insertFromMimeData( source );

    emit pasted();
    QTextCursor c = textCursor();
    c.setPosition( toPlainText().length() - 1 );
    setTextCursor( c );
}

