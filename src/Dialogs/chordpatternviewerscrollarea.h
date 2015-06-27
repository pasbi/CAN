#ifndef CHORDPATTERNVIEWERSCROLLAREA_H
#define CHORDPATTERNVIEWERSCROLLAREA_H

#include <QScrollArea>

class ChordPatternViewerScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    explicit ChordPatternViewerScrollArea(QWidget *parent = 0);

protected:
    void wheelEvent(QWheelEvent *e);
    void keyPressEvent(QKeyEvent* e);

};

#endif // CHORDPATTERNVIEWERSCROLLAREA_H
