#include "creatable.h"


QMap<QString, Creatable* (*)()>  Creatable::m_constructorMap;
QMap<QString, QString>           Creatable::m_categoryMap;


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
