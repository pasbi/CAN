#ifndef PERSISTENTOBJECT_H
#define PERSISTENTOBJECT_H

#include <QJsonObject>

class PersistentObject
{
protected:
    PersistentObject();
    virtual ~PersistentObject();

public:
    // For each class deriving from Persistent, an key equal to its classname is reservated.
    virtual void restoreFromJsonObject( const QJsonObject & object ) = 0;
    virtual QJsonObject toJsonObject() const = 0;
};

#endif // PERSISTENTOBJECT_H
