#ifndef DATEDATABASEWIDGET_H
#define DATEDATABASEWIDGET_H

#include <QWidget>
#include "Database/EventDatabase/eventdatabase.h"
#include "Database/EventDatabase/eventdatabasesortproxy.h"


namespace Ui {
class EventDatabaseWidget;
}

class EventDatabaseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EventDatabaseWidget(QWidget *parent = 0);
    ~EventDatabaseWidget();

    void setEventDatabase( EventDatabase * eventDatabase );
    Event* currentEvent() const;


private:
    Ui::EventDatabaseWidget *ui;
    EventDatabase* m_eventDatabase;
    EventDatabaseSortProxy m_sortFilterProxy;
};

#endif // DATEDATABASEWIDGET_H
