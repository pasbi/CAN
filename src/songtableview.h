#ifndef SONGTABLE_H
#define SONGTABLE_H

#include <QTableView>

class SongTableView : public QTableView
{
    Q_OBJECT
public:
    explicit SongTableView(QWidget *parent = 0);

    bool edit(const QModelIndex &index, EditTrigger trigger, QEvent *event);

};

#endif // SONGTABLE_H
