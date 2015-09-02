#ifndef DATEDATABASEWIDGET_H
#define DATEDATABASEWIDGET_H

#include "DatabaseView/databasewidget.h"

class SetlistView;
class Setlist;
class SetlistWidget;
class Event;
class EventDatabaseWidget : public DatabaseWidget<Event>
{
    Q_OBJECT

public:
    explicit EventDatabaseWidget(QWidget *parent = 0);

    SetlistView *setlistView() const;
    Setlist* currentSetlist() const;

protected:
    void setupUi();

private slots:
    void updateSetlistView();

private:
    SetlistWidget* m_setlistWidget;
};

#endif // DATEDATABASEWIDGET_H
