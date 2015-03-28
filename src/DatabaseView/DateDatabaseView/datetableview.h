#ifndef DATETABLEVIEW_H
#define DATETABLEVIEW_H

#include <QWidget>
#include <QTableView>
#include "Database/DateDatabase/datedatabase.h"

class DateTableView : public QTableView
{
    Q_OBJECT

public:
    explicit DateTableView(QWidget *parent = 0);
    ~DateTableView();
    int sizeHintForColumn(int column) const;

    void setModel(DateDatabase *model);
    DateDatabase* model() const;

protected:
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    bool showDialog(QModelIndex index);

};

#endif // DATETABLEVIEW_H
