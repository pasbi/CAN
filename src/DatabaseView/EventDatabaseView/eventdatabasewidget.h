#ifndef DATEDATABASEWIDGET_H
#define DATEDATABASEWIDGET_H

#include <QWidget>
#include "Database/EventDatabase/eventdatabase.h"

namespace Ui {
class EventDatabaseWidget;
}

class EventDatabaseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EventDatabaseWidget(QWidget *parent = 0);
    ~EventDatabaseWidget();

    void setEventDatabase( EventDatabase * dateDatabase );
    Event* currentEvent() const;


private:
    Ui::EventDatabaseWidget *ui;
    EventDatabase* m_dateDatabase;
};

#endif // DATEDATABASEWIDGET_H
