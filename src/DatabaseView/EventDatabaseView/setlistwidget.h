#ifndef SETLISTWIDGET_H
#define SETLISTWIDGET_H

#include <QWidget>
#include "Database/EventDatabase/setlist.h"
#include <QListView>

namespace Ui {
class SetlistWidget;
}

class SetlistWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SetlistWidget(QWidget *parent = 0);
    ~SetlistWidget();

    void setSetlist( Setlist* Setlist );
    QListView* listView() const;

private slots:
    void on_buttonSortUp_clicked();
    void on_buttonSortDown_clicked();
    void on_buttonDelete_clicked();
    void on_buttonAdd_clicked();

private:
    Ui::SetlistWidget *ui;
    Setlist* m_currentSetlist = NULL;
};

#endif // SETLISTWIDGET_H
