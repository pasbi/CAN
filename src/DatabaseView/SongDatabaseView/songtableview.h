#ifndef SONGTABLE_H
#define SONGTABLE_H

#include "DatabaseView/databaseview.h"
#include "Database/SongDatabase/songdatabase.h"
#include "songattributedelegate.h"
#include "Database/SongDatabase/songdatabasesortproxy.h"


class SongTableView : public DatabaseView
{
    Q_OBJECT
public:
    explicit SongTableView(QWidget *parent = 0);

    void setModel(SongDatabaseSortProxy* model);
    SongDatabase* model() const;
    SongDatabaseSortProxy* proxyModel() const;




public slots:
    void fakeFocusOutEvent();

private:
    void setUpContextMenu(QMenu* menu);

    SongAttributeDelegate* m_delegate;

};

#endif // SONGTABLE_H
