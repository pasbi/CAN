#ifndef SONGTABLEVIEWCONTAINER_H
#define SONGTABLEVIEWCONTAINER_H

#include <QWidget>
#include "songtableview.h"
#include "Database/SongDatabase/songdatabasesortproxy.h"

namespace Ui {
class SongTableViewContainer;
}

class SongTableViewContainer : public QWidget
{
    Q_OBJECT

public:
    explicit SongTableViewContainer(QWidget *parent = 0);
    ~SongTableViewContainer();

    SongTableView* songTableView() const;
    void setModel(SongDatabaseSortProxy *model);

    Song* currentSong() const;

private:
    Ui::SongTableViewContainer *ui;
};

#endif // SONGTABLEVIEWCONTAINER_H
