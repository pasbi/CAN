#include "creatable.h"


QMap<QString, Creatable* (*)()>  Creatable::m_constructorMap;

Creatable::Creatable()
{
}


bool Creatable::create(const QString & classname, void* &object)
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
