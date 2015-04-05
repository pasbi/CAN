#ifndef DATEDATABASEWIDGET_H
#define DATEDATABASEWIDGET_H

#include <QWidget>
#include "Database/EventDatabase/eventdatabase.h"
#include "Database/EventDatabase/eventdatabasesortproxy.h"
#include <QListView>
#include "eventtableview.h"


namespace Ui {
class EventDatabaseWidget;
}

class EventDatabaseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EventDatabaseWidget(QWidget *parent = 0);
    ~EventDatabaseWidget();

    Event* currentEvent() const;
    QListView* setlistView() const;
    EventTableView* eventTableView() const;
    SetlistItem* currentSetlistItem() const;
    Setlist* currentSetlist() const;


private slots:
    void updateSetlistView();

private:
    Ui::EventDatabaseWidget *ui;
};

#endif // DATEDATABASEWIDGET_H
