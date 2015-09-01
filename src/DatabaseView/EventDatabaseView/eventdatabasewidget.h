#ifndef DATEDATABASEWIDGET_H
#define DATEDATABASEWIDGET_H

#include <QWidget>

namespace Ui {
class EventDatabaseWidget;
}

class Event;
class SetlistView;
class EventTableView;
class Setlist;
class EventDatabaseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EventDatabaseWidget(QWidget *parent = 0);
    ~EventDatabaseWidget();

    Event* currentEvent() const;
    SetlistView *setlistView() const;
    EventTableView* eventTableView() const;
    Setlist* currentSetlist() const;


private slots:
    void updateSetlistView();

private:
    Ui::EventDatabaseWidget *ui;
};

#endif // DATEDATABASEWIDGET_H
