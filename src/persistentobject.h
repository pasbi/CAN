#ifndef PERSISTENTOBJECT_H
#define PERSISTENTOBJECT_H

#include <QJsonObject>
#include <QJsonArray>
#include "global.h"


// check if object has a certain key of certain type. return false if not and print warning.
#define checkJsonObject(OBJECT, KEY, TYPE)                                          \
      (( OBJECT.contains(KEY)       || (WARNING << "Expected key " << KEY, false) )        \
    && ( OBJECT[KEY].type() == TYPE || (WARNING << "Expected type: " << TYPE, false)  ))

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

private:
    static QString createRandomID();
protected:
    QString m_randomID;
public:
    QString randomID() const { return m_randomID; }
    static void seedRandomID();

};

#endif // PERSISTENTOBJECT_H
