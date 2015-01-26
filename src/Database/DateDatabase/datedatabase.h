#ifndef DATEDATABASE_H
#define DATEDATABASE_H

#include "Database/database.h"

class DateDatabase : public Database
{
public:
    DateDatabase(Project *project);


    /////////////////////////////////////////////////
    ////
    ///  Converting to and from JsonObject
    //
    /////////////////////////////////////////////////
public:
    bool restoreFromJsonObject(const QJsonObject & object);
    QJsonObject toJsonObject() const;
};

#endif // DATEDATABASE_H
