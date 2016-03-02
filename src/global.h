#ifndef GLOBAL_H
#define GLOBAL_H

#include <QDebug>
#include "util.h"

#define WARNING qWarning() << QString("at %1:%2, in %3: ").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__)

QVariant preference(const QString& key);
void setPreference(const QString& key, const QVariant& value);

class Command;
void pushCommand(Command* command);

void reConnect(QObject* oldSender, QObject* newSender, const char* signal, QObject* receiver, const char* slot);
void reConnect(QObject* sender, const char* signal, QObject* oldReceiver, QObject* newReceiver, const char* slot);

template<typename T> void setPreference(const QString& key, T value)
{
    setPreference(key, QVariant::fromValue<T>(value));
}

template<typename T> T preference(const QString& key)
{
    return preference(key).value<T>();
}

typedef qint32 EnumSurrogate_t;


#define REGISTER_META_TYPE_STREAM_OPERATORS(TYPE, NAME) \
    static struct NAME##RegisterMetaTypeStreamOperators_t \
    { \
        NAME##RegisterMetaTypeStreamOperators_t() \
        { \
            qRegisterMetaTypeStreamOperators<TYPE>(); \
        } \
    } NAME##RegisterMetaTypeStreamOperators;

#define REGISTER_META_TYPE_COMPARE_OPERATORS(TYPE, NAME) \
    static struct NAME##RegisterMetaTypeCompareOperators_t \
    { \
        NAME##RegisterMetaTypeCompareOperators_t() \
        { \
            if (!QMetaType::hasRegisteredComparators<TYPE>()) \
            { \
                QMetaType::registerComparators<TYPE>(); \
            } \
        } \
    } NAME##RegisterMetaTypeCompareOperators;

// this operator is non sense and you should never use it.
// however, QMetaType::registerComparators() needs an implementation.
#define DEFINE_NONSENSE_SMALLER_OPERATOR(TYPE) \
    bool operator <(const TYPE&) const { return false; }

#endif // GLOBAL_H
