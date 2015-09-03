#ifndef DATEDATABASEWIDGET_H
#define DATEDATABASEWIDGET_H

#include "DatabaseView/databasewidget.h"

class SetlistView;
class Setlist;
class SetlistWidget;
class Event;
class EventDatabaseWidget : public DatabaseWidget<Event, SetlistWidget>
{
    Q_OBJECT

public:
    explicit EventDatabaseWidget(QWidget *parent = 0);

    SetlistView *setlistView() const;
    Setlist* currentSetlist() const;

private slots:
    void updateSetlistView();

private:
    SetlistWidget* setlistWidget() const;

};

#endif // DATEDATABASEWIDGET_H
