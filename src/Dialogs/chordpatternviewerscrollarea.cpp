#include "chordpatternviewerscrollarea.h"
#include <QWheelEvent>
#include <QPainter>
#include "global.h"

ChordPatternViewerScrollArea::ChordPatternViewerScrollArea(QWidget *parent) :
    QScrollArea(parent)
{
}

void ChordPatternViewerScrollArea::wheelEvent(QWheelEvent *e)
{
    e->ignore();
}

void ChordPatternViewerScrollArea::keyPressEvent(QKeyEvent *e)
{
    e->ignore();
}
