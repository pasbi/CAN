#ifndef SONGTABLE_H
#define SONGTABLE_H

#include <QTableView>
#include "Database/SongDatabase/songdatabase.h"
#include "songattributedelegate.h"
#include "Database/SongDatabase/songdatabasesortproxy.h"


class SongTableView : public QTableView
{
    Q_OBJECT
public:
    explicit SongTableView(QWidget *parent = 0);

    void setModel(SongDatabaseSortProxy* model);
    SongDatabase* model() const;
    SongDatabaseSortProxy* proxyModel() const;

public slots:
    void fakeFocusOutEvent();

private slots:
    void showContextMenu(QPoint pos);

private:
    void setUpContextMenu(QMenu* menu);
    QModelIndex indexUnderCursor() const;

    SongAttributeDelegate* m_delegate;

};

#endif // SONGTABLE_H
