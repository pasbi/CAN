#include "creatable.h"

QHash<QString, Creatable* (*)()>*    Creatable::m_constructorMap = NULL;
QHash<QString, QString>*             Creatable::m_categoryMap = NULL;
QHash<QString, QString>*             Creatable::m_inverseCategoryMap = NULL;
QHash<QString, const char*>*         Creatable::m_nameMap  = NULL;

Creatable::Creatable()
{
}

QString Creatable::category(const QString &classname)
{
    if (!m_categoryMap->contains(classname))
    {
        return QString();
    }
    else
    {
        return m_categoryMap->value(classname);
    }
}

QStringList Creatable::classnamesInCategory(const QString &category)
{
    return m_inverseCategoryMap->values( category );
}

QString Creatable::name(const QString& classname)
{
    return (*m_nameMap)[classname];
}
