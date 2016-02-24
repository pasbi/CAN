#ifndef EVENTMERGEWIDGET_H
#define EVENTMERGEWIDGET_H

#include <QWidget>
#include "databasemerger.h"
#include "mergetreeview.h"

class Event;

class EventMergeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EventMergeWidget(QWidget *parent = 0);
    ~EventMergeWidget();
    void setDatabase(Database<Event> *master, Database<Event> *slave);
    const QTreeWidgetItem* rootItem() const { return m_mergeTreeView->root(); }


private:
    MergeTreeView* m_mergeTreeView;
    DatabaseMerger<Event>* m_eventDatabaseMerger;


};

#endif // EVENTMERGEWIDGET_H
