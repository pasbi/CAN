#include "song.h"
#include "songdatabase.h"
#include "global.h"

Song::Song(SongDatabase* database) :
    m_songDatabase(database)
{
    int n = m_songDatabase->columnCount();
    m_attributes.reserve(n);
    for (int i = 0; i < n; ++i)
    {
        m_attributes.append(QVariant());
    }
}


bool Song::restoreFromJsonObject(const QJsonObject &json)
{
    if (!Taggable::restoreFromJsonObject(json))
    {
        return false;
    }

    if (!checkJsonObject(json, "attributes", QJsonValue::Array))
    {
        return false;
    }

    QJsonArray attributes = json["attributes"].toArray();
    m_attributes.clear();
    m_attributes.reserve(attributes.size());
    for (const QJsonValue & val : attributes)
    {
        m_attributes.append(val.toVariant());
    }

    qDebug() << "attributes rest: " << m_attributes;

    return true;
}

QJsonObject Song::toJsonObject() const
{
    QJsonObject json = Taggable::toJsonObject();

    QJsonArray array;
    for (const QVariant & v : m_attributes)
    {
        array.append(QJsonValue::fromVariant(v));
    }

    json.insert("attributes", array);

    return json;
}

QString Song::title() const
{
    return attribute(0).toString();
}

void Song::setTitle(const QString &title)
{
    setAttribute(0, title);
}

void Song::setAttribute(int index, const QVariant &data)
{
    assert(index >= 0 && index < m_attributes.length());
    m_attributes[index] = data;
    m_songDatabase->notifyDataChanged( this );
}

void Song::insertAttribute(int index, const QVariant &data)
{
    m_attributes.insert(index, data);
}

QVariant Song::attribute(int index) const
{
    assert(index >= 0 && index < m_attributes.length());
    return m_attributes[index];
}

QVariant& Song::attribute(int index)
{
    qDebug() << "atrs: " << index << m_attributes;
    assert(index >= 0 && index < m_attributes.length());
    return m_attributes[index];
}

void Song::removeAttribute(int index)
{
    m_attributes.removeAt(index);
}
