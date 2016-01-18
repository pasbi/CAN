#ifndef DATETABLEVIEW_H
#define DATETABLEVIEW_H

#include "DatabaseView/databaseview.h"

class Event;
class EventTableView : public DatabaseView<Event>
{
    Q_OBJECT

public:
    explicit EventTableView(QWidget *parent = 0);
    int sizeHintForColumn(int column) const;
    void triggerEditDate(const Event* event);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    bool showDateTimeDialog(QModelIndex index);
};

#endif // DATETABLEVIEW_H
