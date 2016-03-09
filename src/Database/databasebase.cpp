#include "databasebase.h"

DatabaseBase::DatabaseBase(Project *project, QObject* parent) :
    QAbstractTableModel(parent),
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

void DatabaseBase::reset()
{
    emit reseted();
}

QDataStream& operator<<(QDataStream& out, const DatabaseBase* database)
{
    database->serialize(out);
    return out;
}

QDataStream& operator>>(QDataStream& in, DatabaseBase* database)
{
    database->deserialize(in);
    return in;
}
