#include "taggable.h"
#include "global.h"
#include <QJsonArray>

Taggable::Taggable()
{
}

Taggable::~Taggable()
{

}

bool Taggable::restoreFromJsonObject(const QJsonObject& json)
{
    if (!checkJsonObject(json, "tags", QJsonValue::Array))
    {
        return false;
    }

    m_tags.clear();
    for (const QJsonValue & value : json["tags"].toArray())
    {
        if (value.isString())
        {
            m_tags.insert(value.toString());
        }
        else
        {
            WARNING << "Expected String";
            return false;
        }
    }

    return true;
}

QJsonObject Taggable::toJsonObject() const
{
    QJsonObject json;
    QJsonArray tags;
    for (const QString & tag : m_tags)
    {
        tags.append(tag);
    }

    json.insert("tags", tags);
    return json;
}
