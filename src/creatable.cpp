#include "creatable.h"
#include "global.h"

QHash<QString, Creatable* (*)()>                    Creatable::m_constructorMap;
QHash<QString, QString>                             Creatable::m_categoryMap;
QHash<QString, QString>                             Creatable::m_inverseCategoryMap;
QHash<QString, QString>                             Creatable::m_nameMap;

Creatable::Creatable()
{
}


bool Creatable::create(const QString & classname, Creatable* &object)
{
    if (!m_constructorMap.contains(classname))
    {
        return false;
    }
    else
    {
        object = m_constructorMap.value(classname)();
        return true;
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

QString Creatable::name(const QString &classname)
{
    qDebug() << m_nameMap;
    return m_nameMap[classname];
}
