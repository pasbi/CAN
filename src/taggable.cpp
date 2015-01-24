#include "taggable.h"
#include "global.h"
#include <QJsonArray>

Taggable::Taggable()
{
}

Taggable::~Taggable()
{

}

void Taggable::restoreFromJsonObject(const QJsonObject& json)
{
    if (json["Taggable"].isArray())
    {
        m_tags.clear();
        for (const QJsonValue & value : json["Taggable"].toArray())
        {
            if (value.isString())
            {
                m_tags.insert(value.toString());
            }
            else
            {
                WARNING << "Expected String";
            }
        }
    }
    else
    {
        WARNING << "Expected key \"Taggable\"";
    }
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
