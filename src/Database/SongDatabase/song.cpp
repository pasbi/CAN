#include "song.h"

Song::Song()
{
}


void Song::restoreFromJsonObject(const QJsonObject &json)
{
    Taggable::restoreFromJsonObject(json);
}

QJsonObject Song::toJsonObject() const
{
    QJsonObject json = Taggable::toJsonObject();

    return json;
}

QString Song::title() const
{
    return m_title;
}

void Song::setTitle(const QString &title)
{
    m_title = title;
}
