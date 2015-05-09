#ifndef DATABASEVIEW_H
#define DATABASEVIEW_H

#include <QTableView>

class DatabaseView : public QTableView
{
    Q_OBJECT
public:
    explicit DatabaseView(QWidget *parent = 0);

    QModelIndex indexUnderCursor() const;



};

#endif // DATABASEVIEW_H
