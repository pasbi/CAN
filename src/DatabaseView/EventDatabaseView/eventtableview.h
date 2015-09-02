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

protected:
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    bool showDialog(QModelIndex index);

    QAction* m_actionNew_Event;
    QAction* m_actionDelete_Event;
};

#endif // DATETABLEVIEW_H
