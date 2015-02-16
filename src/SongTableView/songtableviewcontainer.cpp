#include "songtableviewcontainer.h"
#include "ui_songtableviewcontainer.h"
#include <QLineEdit>
#include <QPushButton>
#include "global.h"

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
//    connect(ui->comboBox, SIGNAL(editTextChanged(QString)), model, SLOT(setFilterWildcard(QString)));
//    connect(ui->comboBox, static_cast< void (QComboBox::*)(int) >( &QComboBox::currentIndexChanged), [this](int i)
//    {
//        if (i == -1)
//            ui->tableView->proxyModel()->setFilterWildcard("");
//    });
}

Song* SongTableViewContainer::currentSong() const
{
    QModelIndexList rows = ui->tableView->selectionModel()->selectedRows();
    if (rows.isEmpty())
    {
        return NULL;
    }
    return ui->tableView->model()->songAtIndex( rows.first() );
}
