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
