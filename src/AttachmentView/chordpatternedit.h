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
    void keyPressEvent(QKeyEvent *e);
    void insertFromMimeData(const QMimeData *source);

private:
    QMenu* m_contextMenu = NULL;

signals:
    void pasted();


};

#endif // CHORDPATTERNEDIT_H
