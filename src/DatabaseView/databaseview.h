#ifndef DATABASEVIEW_H
#define DATABASEVIEW_H

#include <QTableView>
#include "taggable.h"

class DatabaseView : public QTableView
{
    Q_OBJECT
public:
    explicit DatabaseView(QWidget *parent = 0);

    QModelIndex indexUnderCursor() const;
    virtual Taggable* objectAt(const QModelIndex& index) = 0;



};

#endif // DATABASEVIEW_H
