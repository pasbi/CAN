#include "creatable.h"

QHash<QString, Creatable* (*)()>*    Creatable::m_constructorMap = nullptr;
QHash<QString, QString>*             Creatable::m_categoryMap = nullptr;
QHash<QString, QString>*             Creatable::m_inverseCategoryMap = nullptr;
QHash<QString, const char*>*         Creatable::m_nameMap  = nullptr;

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
