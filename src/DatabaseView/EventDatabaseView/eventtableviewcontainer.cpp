#include "eventtableviewcontainer.h"

#include "DatabaseView/EventDatabaseView/eventtableviewcontainer.h"
#include "DatabaseView/EventDatabaseView/eventtableview.h"
#include "Database/EventDatabase/eventdatabase.h"

EventTableViewContainer::EventTableViewContainer(QWidget *parent) :
    DatabaseViewContainer(parent)
{
    m_databaseView = new EventTableView(this);
    setupUi();
}
