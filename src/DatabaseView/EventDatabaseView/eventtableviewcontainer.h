#ifndef EVENTTABLEVIEWCONTAINER_H
#define EVENTTABLEVIEWCONTAINER_H

#include <QWidget>

namespace Ui {
class EventTableViewContainer;
}

class EventTableView;
class EventDatabaseSortProxy;
class Event;
class EventTableViewContainer : public QWidget
{
    Q_OBJECT

public:
    explicit EventTableViewContainer(QWidget *parent = 0);
    ~EventTableViewContainer();

    EventTableView* eventTableView() const;
    void setModel( EventDatabaseSortProxy* model );
    Event* currentEvent() const;

private:
    Ui::EventTableViewContainer *ui;
};

#endif // EVENTTABLEVIEWCONTAINER_H
