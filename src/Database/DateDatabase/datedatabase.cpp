#include "datedatabase.h"

DateDatabase::DateDatabase(Project *project) :
    Database(project)
{
}


QJsonObject DateDatabase::toJsonObject() const
{
    return QJsonObject();
}

bool DateDatabase::restoreFromJsonObject(const QJsonObject &object)
{

}
