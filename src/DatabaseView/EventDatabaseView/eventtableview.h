#ifndef DATETABLEVIEW_H
#define DATETABLEVIEW_H

#include "DatabaseView/databaseview.h"

class EventDatabaseSortProxy;
template<typename T> class Database;
class Event;

class EventTableView : public DatabaseView<Event>
{
    Q_OBJECT

public:
    explicit EventTableView(QWidget *parent = 0);
    ~EventTableView();
    int sizeHintForColumn(int column) const;

    void setModel(EventDatabaseSortProxy *model);
    Database<Event>* model() const;
    EventDatabaseSortProxy* proxyModel() const;

protected:
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    bool showDialog(QModelIndex index);
    void setUpContextMenu(QMenu* menu);

    QAction* m_actionNew_Event;
    QAction* m_actionDelete_Event;


};

#endif // DATETABLEVIEW_H
