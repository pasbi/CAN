#include "persistentobject.h"
#include <QFile>
#include <QJsonDocument>
#include "global.h"

PersistentObject::PersistentObject()
{
}

PersistentObject::~PersistentObject()
{
}

bool PersistentObject::loadFrom(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        WARNING << "Cannot open file " << path << " for reading.";
        return false;
    }

    QByteArray fileContent = file.readAll();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(fileContent, &error);
    if (error.error != QJsonParseError::NoError)
    {
        WARNING << "Cannot parse " << path << ": " << error.errorString();
        return false;
    }

    if (!doc.isObject())
    {
        WARNING << "Expected JsonObject\n" << QString(doc.toJson());
        return false;
    }

    if (!restoreFromJsonObject(doc.object()))
    {
        WARNING << "Cannot restore object\n" << QString(doc.toJson());
        return false;
    }
    return true;
}

bool PersistentObject::saveTo(const QString &path) const
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly))
    {
        WARNING << "Cannot open file " << path << " for writing.";
        return false;
    }

    QJsonDocument doc(toJsonObject());
    file.write(doc.toJson());

    return true;
}
