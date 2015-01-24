#ifndef DATABASE_H
#define DATABASE_H

class Project;
class Database
{
public:
    Database(Project* project);
    Project* project() const { return m_project; }

private:
    Project* m_project;
};

#endif // DATABASE_H
