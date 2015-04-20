#ifndef CHORDPATTERNEDIT_H
#define CHORDPATTERNEDIT_H

#include <QTextEdit>

class ChordPatternEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit ChordPatternEdit(QWidget *parent = 0);

protected:
    void contextMenuEvent(QContextMenuEvent *e);
    bool eventFilter(QObject *o, QEvent *e);

private:
    QMenu* m_contextMenu = NULL;


};

#endif // CHORDPATTERNEDIT_H
