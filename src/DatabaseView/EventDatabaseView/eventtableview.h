#ifndef DATETABLEVIEW_H
#define DATETABLEVIEW_H

#include "DatabaseView/databaseview.h"
#include "Database/EventDatabase/eventdatabase.h"
#include "Database/EventDatabase/eventdatabasesortproxy.h"

class EventTableView : public DatabaseView
{
    Q_OBJECT

public:
    explicit EventTableView(QWidget *parent = 0);
    ~EventTableView();
    int sizeHintForColumn(int column) const;

    void setModel(EventDatabaseSortProxy *model);
    EventDatabase* model() const;
    EventDatabaseSortProxy* proxyModel() const;

    void pasteEvents(const QMimeData *mimeData, int row, Qt::DropAction action);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    Taggable* objectAt(const QModelIndex &index);


private:
    bool showDialog(QModelIndex index);
    void setUpContextMenu(QMenu* menu);

    QAction* m_actionNew_Event;
    QAction* m_actionDelete_Event;

    static Qt::DropAction dropAction( QDropEvent* event );


};

#endif // DATETABLEVIEW_H
