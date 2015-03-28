#ifndef DATETABLEVIEW_H
#define DATETABLEVIEW_H

#include <QWidget>
#include <QTableView>

class DateTableView : public QTableView
{
    Q_OBJECT

public:
    explicit DateTableView(QWidget *parent = 0);
    ~DateTableView();

};

#endif // DATETABLEVIEW_H
