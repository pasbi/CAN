#ifndef DATEDATABASEWIDGET_H
#define DATEDATABASEWIDGET_H

#include <QWidget>

namespace Ui {
class EventDatabaseWidget;
}

class Event;
class SetlistView;
class EventTableViewContainer;
template<typename T> class DatabaseView;
class Setlist;
class SetlistWidget;
class EventDatabaseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EventDatabaseWidget(QWidget *parent = 0);
    ~EventDatabaseWidget();

    Event* currentEvent() const;
    SetlistView *setlistView() const;
    DatabaseView<Event>* databaseView() const;
    Setlist* currentSetlist() const;

protected:
    void setupUi();

private slots:
    void updateSetlistView();

private:
    Ui::EventDatabaseWidget *ui;

    EventTableViewContainer* m_tableViewContainer;
    SetlistWidget* m_setlistWidget;
};

#endif // DATEDATABASEWIDGET_H
