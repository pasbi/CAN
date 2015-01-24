#ifndef SONGTABLE_H
#define SONGTABLE_H

#include <QTableView>
#include "Database/SongDatabase/songdatabase.h"
#include <QMouseEvent>

class SongTableView : public QTableView
{
    Q_OBJECT
public:
    explicit SongTableView(QWidget *parent = 0);

    SongDatabase* model() const { return static_cast<SongDatabase*>( QTableView::model() ); }

private slots:
    void showContextMenu();
    void setUpContextMenu(QMenu* menu);
    QModelIndex indexUnderCursor() const;

};

#endif // SONGTABLE_H
