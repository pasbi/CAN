#ifndef EVENTDETAILVIEW_H
#define EVENTDETAILVIEW_H

#include <QWidget>
#include "Database/EventDatabase/event.h"

namespace Ui {
class EventDetailView;
}

class EventDetailView : public QWidget
{
    Q_OBJECT

public:
    explicit EventDetailView(QWidget *parent = 0);
    ~EventDetailView();
    void setEvent(Event* event);

private:
    Ui::EventDetailView *ui;

    QDateTime getDateTime(QDateTime current);
    qint64 m_duration = 0;
    void computeDuration();


    Event* m_event;

private slots:
    void setBeginningDateTime(QDateTime beginning, bool keepDuration = true);
    void setEndingDateTime(QDateTime ending);
};

#endif // EVENTDETAILVIEW_H
