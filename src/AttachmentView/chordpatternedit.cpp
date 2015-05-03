#include "chordpatternedit.h"
#include "global.h"
#include <QKeyEvent>
#include <QAction>
#include "application.h"
#include <QMenu>
#include <QDesktopWidget>
#include <QRect>

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

bool ChordPatternEdit::eventFilter(QObject *o, QEvent *e)
{
    if (e->type() == QEvent::KeyPress)
    {
        // catch undo and redo shortcuts and delegate to application's undo()/redo().
        // Does not work with QShortcutEvent or QActions.
        QKeyEvent* se = dynamic_assert_cast<QKeyEvent*>( e );
        if ( se->modifiers() | Qt::CTRL )
        {
            if ( se->key() == Qt::Key_Z)
            {
                app().undo();
                return true;
            }
            else if ( se->key() == Qt::Key_Y )
            {
                app().redo();
                return true;
            }
        }
    }
    return QTextEdit::eventFilter(o, e);
}

