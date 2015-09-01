#include "songtableviewcontainer.h"

#include <QLineEdit>
#include <QPushButton>

#include "ui_songtableviewcontainer.h"
#include "global.h"
#include "songtableview.h"
#include "Database/SongDatabase/songdatabasesortproxy.h"

SongTableViewContainer::SongTableViewContainer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SongTableViewContainer)
{
    ui->setupUi(this);
}

SongTableViewContainer::~SongTableViewContainer()
{
    delete ui;
}

SongTableView* SongTableViewContainer::songTableView() const
{
    return ui->tableView;
}

void SongTableViewContainer::setModel(SongDatabaseSortProxy *model)
{
    ui->tableView->setModel(model);
    connect(ui->filterWidget, SIGNAL(filterChanged(QString)), model, SLOT(setFilterWildcard(QString)));
}

Song* SongTableViewContainer::currentSong() const
{
    QModelIndexList rows = ui->tableView->selectionModel()->selectedRows();
    if (rows.isEmpty())
    {
        return NULL;
    }
    else
    {
        return ui->tableView->model()->itemAtIndex( rows.first() );
    }
}
