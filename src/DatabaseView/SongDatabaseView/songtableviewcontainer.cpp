#include "songtableviewcontainer.h"

#include "DatabaseView/SongDatabaseView/songtableviewcontainer.h"
#include "DatabaseView/SongDatabaseView/songtableview.h"
#include "Database/SongDatabase/songdatabase.h"

SongTableViewContainer::SongTableViewContainer(QWidget *parent) :
    DatabaseViewContainer(parent)
{
    m_databaseView = new SongTableView(this);
    setupUi();
}
