#ifndef EVENTMERGEWIDGET_H
#define EVENTMERGEWIDGET_H

#include <QWidget>
#include "databasemerger.h"
#include "mergeitem.h"
#include "mergelistview.h"

class Event;

class EventMergeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EventMergeWidget(QWidget *parent = 0);
    ~EventMergeWidget();
    void setDatabase(Database<Event> *master, Database<Event> *slave);
    QList<MergeItemBase> mergeItems() const { return m_mergeListView->mergeItems(); }

private:
    MergeListView* m_mergeListView;
    DatabaseMerger<Event>* m_eventDatabaseMerger;


};

#endif // EVENTMERGEWIDGET_H
