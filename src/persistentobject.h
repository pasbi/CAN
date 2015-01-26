#ifndef PERSISTENTOBJECT_H
#define PERSISTENTOBJECT_H

#include <QJsonObject>
#include <QJsonArray>
#include "global.h"

#define checkJsonObject(OBJECT, KEY, TYPE)                                          \
      (( OBJECT.contains(KEY)       || (WARNING << "Expected key " << KEY) )        \
    && ( OBJECT[KEY].type() == TYPE || (WARNING << "Expected type: " << TYPE) )  )

class PersistentObject
{
protected:
    PersistentObject();
    virtual ~PersistentObject();

protected:
    // For each class deriving from Persistent, an key equal to its classname is reservated.
    virtual bool restoreFromJsonObject( const QJsonObject & object ) = 0;
    virtual QJsonObject toJsonObject() const = 0;

public:
    virtual bool saveTo(const QString & path) const;
    virtual bool loadFrom(const QString & path);
};

#endif // PERSISTENTOBJECT_H
