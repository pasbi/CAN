#ifndef PERSISTENTOBJECT_H
#define PERSISTENTOBJECT_H

#include <QJsonObject>
#include <QJsonArray>
#include "global.h"


class File;
class PersistentObject
{
protected:
    PersistentObject();
    virtual ~PersistentObject();

protected:
    virtual void serialize(QDataStream& out) const = 0;
    virtual void deserialize(QDataStream& in) = 0;
    QMap<QString, QVariant> m_persistentProperties;

public:
    bool loadFrom(const QString & path);

    static const Qt::DateFormat DATE_TIME_FORMAT;

    friend QDataStream& operator<<(QDataStream& out, const PersistentObject* object);
    friend QDataStream& operator>>(QDataStream& in, PersistentObject* object);
};

QDataStream& operator<<(QDataStream& out, const PersistentObject* object);
QDataStream& operator>>(QDataStream& in, PersistentObject* object);

#endif // PERSISTENTOBJECT_H
