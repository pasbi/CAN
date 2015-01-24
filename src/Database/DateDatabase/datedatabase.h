#ifndef DATEDATABASE_H
#define DATEDATABASE_H

#include "Database/database.h"

class DateDatabase : public Database
{
public:
    DateDatabase(Project *project);
};

#endif // DATEDATABASE_H
