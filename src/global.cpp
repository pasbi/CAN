#include "global.h"
#include "application.h"
#include "Commands/command.h"

QVariant preference(const QString &key)
{
    return app().preferences()->get(key)->value();
}

void setPreference(const QString &key, const QVariant& value)
{
    app().preferences()->get(key)->setValue(value);
}

void pushCommand(Command *command)
{
    app().pushCommand(command);
}

void reConnect(QObject* oldSender, QObject* newSender, const char* signal, QObject* receiver, const char* slot)
{
    if (oldSender)
    {
        QObject::disconnect(oldSender, signal, receiver, slot);
    }
    if (newSender)
    {
        QObject::connect(newSender, signal, receiver, slot);
    }
}

void reConnect(QObject* sender, const char* signal, QObject* oldReceiver, QObject* newReceiver, const char* slot)
{
    if (oldReceiver)
    {
        QObject::disconnect(sender, signal, oldReceiver, slot);
    }
    if (newReceiver)
    {
        QObject::connect(sender, signal, newReceiver, slot);
    }
}
