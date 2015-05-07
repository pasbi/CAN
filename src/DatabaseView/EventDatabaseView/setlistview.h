#ifndef SETLISTVIEW_H
#define SETLISTVIEW_H

#include <QTableView>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include "Database/EventDatabase/setlist.h"
#include <QMenu>

class SetlistView : public QTableView
{
    Q_OBJECT
public:
    explicit SetlistView(QWidget *parent = 0);

    Setlist* model() const { return qobject_assert_cast<Setlist*>( QTableView::model() ); }
    void setModel(Setlist* setlist);
    void paste(const QMimeData* mime);

signals:
    void clicked();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *e);
    void dropEvent(QDropEvent *e);
    void mousePressEvent(QMouseEvent *event);

private slots:
    void updateMinimumHorizontalHeaderSize();
    void showContextMenu(QPoint pos);

private:
    void setUpContextMenu(QMenu* menu, QPoint pos);


public:
    static const QString ITEMS_MIMEDATA_FORMAT;


};

#endif // SETLISTVIEW_H
