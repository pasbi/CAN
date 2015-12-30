#include "persistentobject.h"
#include <QFile>
#include <QJsonDocument>
#include "global.h"
#include <QDateTime>

const Qt::DateFormat PersistentObject::DATE_TIME_FORMAT = Qt::ISODate;


PersistentObject::PersistentObject()
{
}

PersistentObject::~PersistentObject()
{
}

QDataStream& operator<<(QDataStream& out, const PersistentObject* object)
{
    object->serialize(out);
    return out;
}

QDataStream& operator>>(QDataStream& in, PersistentObject* object)
{
    object->deserialize(in);
    return in;
}
