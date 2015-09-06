#include "database.h"

DatabaseBase::DatabaseBase(Project *project, QObject* parent) :
    QAbstractTableModel(parent),
    PersistentObject(),
    m_project(project)
{

}

DatabaseBase::~DatabaseBase()
{

}

Project* DatabaseBase::project() const
{
    return m_project;
}

Qt::DropActions DatabaseBase::supportedDropActions() const
{
    return Qt::IgnoreAction;
}
