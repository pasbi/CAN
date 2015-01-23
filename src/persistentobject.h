#ifndef PERSISTENTOBJECT_H
#define PERSISTENTOBJECT_H

#include <QJsonObject>

class PersistentObject
{
protected:
    PersistentObject();

public:
    bool restoreFromQJsonObject( const QJsonObject & object );
    QJsonObject toQJsonObject() const;
};

#endif // PERSISTENTOBJECT_H
