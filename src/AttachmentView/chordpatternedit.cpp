#include "chordpatternedit.h"
#include "global.h"
#include <QKeyEvent>
#include <QAction>
#include "application.h"
#include <QMenu>
#include <QDesktopWidget>
#include <QRect>
#include <QMimeData>
#include <QScrollBar>
#include <QClipboard>


void insertSorted( QList<int>& list, int n )
{
    int i = 0;
    while (i < list.length() && list[i] < n)
    {
        i++;
    }
    list.insert(i, n);
}

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

void ChordPatternEdit::clearHighlights()
{
    m_selectedLines.clear();
    updateHighlights();
}

void ChordPatternEdit::keyPressEvent(QKeyEvent* e)
{
    if (e->modifiers() == Qt::NoModifier)
    {
        clearHighlights();
    }

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

        // remember scroll position
        int scrollX, scrollY;
        scrollY = verticalScrollBar()->value();
        scrollX = horizontalScrollBar()->value();

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

        // restore cursor position
        QTextCursor cursor = textCursor();
        cursor.setPosition( pos + numberOfWhitespaces );
        setTextCursor( cursor );

        // restore scrollbars
        verticalScrollBar()->setValue( scrollY );
        horizontalScrollBar()->setValue( scrollX );

        // do not insert actual tab.
        e->accept();
        return;
    }

    return QTextEdit::keyPressEvent(e);
}

void ChordPatternEdit::setExternalExtraSelections( const QList<ExtraSelection>& selections )
{
    m_externalExtraSelections = selections;
    setExtraSelections( m_externalExtraSelections + m_internalExtraSelections );
}

void ChordPatternEdit::setInternalExtraSelections( const QList<ExtraSelection>& selections )
{
    m_internalExtraSelections = selections;
    setExtraSelections( m_externalExtraSelections + m_internalExtraSelections );
}

// overwrite this method just to make it private.
void ChordPatternEdit::setExtraSelections(const QList<ExtraSelection> &selections)
{
    QTextEdit::setExtraSelections( selections );
}

/**
 * @brief line returns the line number of position
 * @param text text
 * @param position position
 * @return  line number
 */
int lineNumber( const QString& text, int position )
{
    int l = 0;
    for (int i = 0; i < position; ++i)
    {
        if (text[i] == '\n')
        {
            l++;
        }
    }
    return l;
}

/**
 * @brief lines returns a list lines, represented by their start, end positions.
 * @param text
 * @return
 */
QList<QPair<int, int>> lineAnchors( const QString& text )
{
    QList<QPair<int, int>> pos;
    QStringList ls = text.split("\n");
    for (const QString& line : ls )
    {
        pos << qMakePair(0, line.length());
    }

    for (int i = 1; i < pos.length(); ++i)
    {
        pos[i].first  += pos[i-1].second + 1;   // dont forget the newline character!
        pos[i].second += pos[i-1].second + 1;
    }

    return pos;
}

void ChordPatternEdit::updateHighlights()
{
    QList<ExtraSelection> highlights;
    QList<QPair<int, int>> linepos = lineAnchors( toPlainText() );
    for (int i : m_selectedLines)
    {
        ExtraSelection highlight;
        QTextCursor cursor = textCursor();
        cursor.setPosition( linepos[i].first );
        cursor.setPosition( linepos[i].second, QTextCursor::KeepAnchor );
        highlight.cursor = cursor;
        QColor color = palette().color( QPalette::Highlight );
        color.setAlpha( 80 );
        highlight.format.setBackground( color );
        highlights << highlight;
    }

    setInternalExtraSelections( highlights );
}

/**
 * @brief line return the line at linenumber in text.
 * @param text
 * @param lineNumber
 * @return
 */
QString line(const QString& text, int lineNumber)
{
    QStringList lines = text.split('\n');
    if (lineNumber >= lines.length() || lineNumber < 0)
    {
        return "";
    }
    else
    {
        return lines[lineNumber];
    }
}

void ChordPatternEdit::mousePressEvent(QMouseEvent *e)
{
    if ((e->button() == Qt::LeftButton) && (e->modifiers() & Qt::CTRL))
    {
        QString plainText = toPlainText();
        QTextCursor cursor = cursorForPosition( e->pos() );
        int n = lineNumber( plainText, cursor.position() );
        if (m_selectedLines.contains(n))
        {
            m_selectedLines.removeOne(n);
        }
        else if ( !line(plainText, n).isEmpty() )   // do not insert empty lines.
        {
            insertSorted( m_selectedLines, n );
            cursor.movePosition( QTextCursor::StartOfLine, QTextCursor::MoveAnchor );
            cursor.movePosition( QTextCursor::EndOfLine, QTextCursor::KeepAnchor );
            setTextCursor( cursor );
        }

        updateHighlights();
    }
    else
    {
        clearHighlights();
        QTextEdit::mousePressEvent(e);
    }
}


void ChordPatternEdit::insertFromMimeData(const QMimeData *source)
{
    clearHighlights();
    if (source->hasText())
    {
        // the easy case: standard pasting
        QTextEdit::insertFromMimeData( source );

        emit pasted();
//        QTextCursor c = textCursor();
//        c.setPosition( toPlainText().length() - 1 );
//        setTextCursor( c );
    }
    else if (source->formats().contains("text/lines"))
    {
        // harder case: paste the lines but leave the lines in between.
        LooseLines lines;
        QDataStream stream(source->data( "text/lines" ));
        stream >> lines;

        QString plainText = toPlainText();
        int n = lineNumber( plainText, textCursor().position());

        // for better user experience, if cursor is on last position in this line, set n to next line.
        if ( plainText[textCursor().position()] == '\n')
        {
            n++;
        }

        QTextCursor cursor = textCursor();
        int pos = cursor.position();
        QString newText = pasteLooseLines( plainText, lines, n );
        int add = newText.length() - plainText.length();

        setText( newText );
        cursor.setPosition( pos + add + 1 );

        setTextCursor( cursor );
        qDebug() << "fancy copy";

        updateHighlights();
    }
}

bool ChordPatternEdit::canInsertFromMimeData(const QMimeData *source) const
{
    // do not rely on base class implementation. We really want to paste purely
    // text and our own format.
    if ( source->formats().contains("text/lines") )
    {
        return true;
    }
    if (source->hasText())
    {
        return true;
    }
    return false;
}

QMimeData* ChordPatternEdit::createMimeDataFromSelection() const
{
    if (m_selectedLines.isEmpty())
    {
        // easy case: copy plain text
        return QTextEdit::createMimeDataFromSelection();
    }
    else
    {
        // hard case. copy selected lines and the number of lines in between.
        QString plainText = toPlainText();
        LooseLines lines;
        for (int i = 0; i < m_selectedLines.length() - 1; ++i)
        {
            int n = m_selectedLines[i];
            QString content = line(plainText, n);
            int spaceToNext = m_selectedLines[i + 1] - n;
            lines.add( content, spaceToNext );
        }
        lines.add( line(plainText, m_selectedLines.last()), -1 );   // finally add last line with arbitrary nextTo-value
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream << lines;

        QMimeData* mimeData = new QMimeData();
        mimeData->setData( "text/lines", data );
        return mimeData;
    }
}

//("a", "b").insert(4, "d") == ("a", "b", "d")
//insert( ("a", "b"), 4, "d" ) == ("a", "b", "", "d")
void insert(QStringList& list, int pos, const QString & item )
{
    while (pos > list.length())
    {
        list.append("");
    }
    list.insert(pos, item);
}

QString ChordPatternEdit::pasteLooseLines(const QString &base, const LooseLines &looseLines, int currentLineNumber)
{
    QStringList lines = base.split("\n");

    int i = currentLineNumber;
    for (const Line& line : looseLines)
    {
        insert( lines, i, line.content );
        i += line.spaceToNext;
    }

    return lines.join("\n");
}
