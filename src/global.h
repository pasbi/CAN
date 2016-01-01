#ifndef GLOBAL_H
#define GLOBAL_H

#include <QDebug>
#include "util.h"

#define WARNING qWarning() << QString("at %1:%2, in %3: ").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__)

QVariant preference(const QString& key);
void setPreference(const QString& key, const QVariant& value);

class Command;
void pushCommand(Command* command);

template<typename T> void setPreference(const QString& key, T value)
{
    setPreference(key, QVariant::fromValue<T>(value));
}

template<typename T> T preference(const QString& key)
{
    return preference(key).value<T>();
}

#endif // GLOBAL_H
