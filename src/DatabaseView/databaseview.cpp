#include "databaseview.h"

DatabaseViewBase::DatabaseViewBase(QWidget* parent) :
    QTableView(parent)
{

}

void DatabaseViewBase::mousePressEvent(QMouseEvent *event)
{
    QTableView::mousePressEvent(event);
    emit clicked();
}
