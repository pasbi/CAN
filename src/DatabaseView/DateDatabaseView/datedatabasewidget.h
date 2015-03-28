#ifndef DATEDATABASEWIDGET_H
#define DATEDATABASEWIDGET_H

#include <QWidget>
#include "Database/DateDatabase/datedatabase.h"

namespace Ui {
class DateDatabaseWidget;
}

class DateDatabaseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DateDatabaseWidget(QWidget *parent = 0);
    ~DateDatabaseWidget();

    void setDateDatabase( DateDatabase * dateDatabase );

private:
    Ui::DateDatabaseWidget *ui;
    DateDatabase* m_dateDatabase;
};

#endif // DATEDATABASEWIDGET_H
