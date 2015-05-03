#ifndef DATABASE_H
#define DATABASE_H

#include "persistentobject.h"

class Project;
class Database : public PersistentObject
{
public:
    Database(Project* project);
    Project* project() const { return m_project; }

private:
    Project* m_project;
};

#endif // DATABASE_H
