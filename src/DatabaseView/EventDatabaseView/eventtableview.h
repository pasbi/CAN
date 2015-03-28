#ifndef DATETABLEVIEW_H
#define DATETABLEVIEW_H

#include <QWidget>
#include <QTableView>
#include "Database/EventDatabase/eventdatabase.h"

class EventTableView : public QTableView
{
    Q_OBJECT

public:
    explicit EventTableView(QWidget *parent = 0);
    ~EventTableView();
    int sizeHintForColumn(int column) const;

    void setModel(EventDatabase *model);
    EventDatabase* model() const;

protected:
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    bool showDialog(QModelIndex index);

};

#endif // DATETABLEVIEW_H
