#include "creatable.h"

QHash<QString, Creatable* (*)()>                    Creatable::m_constructorMap;
QHash<QString, QString>                             Creatable::m_categoryMap;
QHash<QString, QString>                             Creatable::m_inverseCategoryMap;
QHash<QString, const char*>                         Creatable::m_nameMap;

Creatable::Creatable()
{
}

Creatable* Creatable::create(const QString & classname)
{
    if (!m_constructorMap.contains(classname))
    {
        return NULL;
    }
    else
    {
        return m_constructorMap.value(classname)();
    }
}

QString Creatable::category(const QString &classname)
{
    if (!m_categoryMap.contains(classname))
    {
        return QString();
    }
    else
    {
        return m_categoryMap.value(classname);
    }
}

QStringList Creatable::classnamesInCategory(const QString &category)
{
    return m_inverseCategoryMap.values( category );
}

QString Creatable::name(const QString& classname)
{
    return m_nameMap[classname];
}
