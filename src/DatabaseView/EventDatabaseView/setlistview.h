#ifndef SETLISTVIEW_H
#define SETLISTVIEW_H

#include <QListView>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include "Database/EventDatabase/setlist.h"

class SetlistView : public QListView
{
    Q_OBJECT
public:
    explicit SetlistView(QWidget *parent = 0);

    Setlist* model() const { return qobject_assert_cast<Setlist*>( QListView::model() ); }
    void setModel(Setlist* setlist) { QListView::setModel( setlist ); }

    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *e);
    void dropEvent(QDropEvent *e);

};

#endif // SETLISTVIEW_H
