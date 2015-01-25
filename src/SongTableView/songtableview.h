#ifndef SONGTABLE_H
#define SONGTABLE_H

#include <QTableView>
#include "Database/SongDatabase/songdatabase.h"
#include <QMouseEvent>
#include "songattributedelegate.h"


class SongTableView : public QTableView
{
    Q_OBJECT
public:
    explicit SongTableView(QWidget *parent = 0);

    SongDatabase* model() const { return static_cast<SongDatabase*>( QTableView::model() ); }

private slots:
    void showContextMenu(QPoint pos);

private:
    void setUpContextMenu(QMenu* menu);
    QModelIndex indexUnderCursor() const;

    SongAttributeDelegate* m_delegate;

};

#endif // SONGTABLE_H
