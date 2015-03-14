#include "song.h"
#include "songdatabase.h"
#include "global.h"
#include <QJsonDocument>

Song::Song(SongDatabase* database) :
    m_songDatabase(database)
{
    int n = m_songDatabase->columnCount();
    m_attributes.reserve(n);
    for (int i = 0; i < n; ++i)
    {
        m_attributes.append(QString());
    }
}

Song::~Song()
{
    qDeleteAll( m_attachments );
}

QMap<QString, QString> Song::stringAttributes() const
{
    QMap<QString, QString> map;
    assert( m_attributes.length() <= database()->attributeKeys().length() );
    for (int i = 0; i < m_attributes.size(); ++i)
    {
        QString s = m_attributes[i].toString();
        if (!s.isEmpty())
        {
            map.insert( database()->attributeKeys()[i], s );
        }
    }
    return map;
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
        m_attributes.append(val.toString());
    }

    QJsonArray attachments = json["attachments"].toArray();
    m_attachments.clear();
    m_attachments.reserve(attachments.size());
    for (const QJsonValue & val : attachments)
    {
        Attachment* a = NULL;
        if (Attachment::create( val.toObject(), a ))
        {
            a->setSong( this );
            m_attachments.append( a );
            connectAttachment(a);
        }
    }

    return true;
}

QJsonObject Song::toJsonObject() const
{
    QJsonObject json = Taggable::toJsonObject();

    QJsonArray attributes;
    for (const QVariant & v : m_attributes)
    {
        attributes.append(QJsonValue::fromVariant(v));
    }
    json.insert("attributes", attributes);

    QJsonArray attachments;
    for (const Attachment* a : m_attachments)
    {
        QJsonObject o = a->toJsonObject();
        attachments.append( o );
    }
    json.insert("attachments", attachments);



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
    if (m_attributes[index] != data)
    {
        m_attributes[index] = data;
    }

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
    assert(index >= 0 && index < m_attributes.length());
    return m_attributes[index];
}

void Song::removeAttribute(int index)
{
    m_attributes.removeAt(index);
}

QStringList Song::attachmentNames() const
{
    QStringList akk;
    for (const Attachment* a : m_attachments)
    {
        akk << a->name();
    }
    return akk;
}


int Song::removeAttachment( Attachment* attachment )
{
    int index = m_attachments.indexOf( attachment );
    m_attachments.removeOne( attachment );
    emit attachmentRemoved( index );
    return index;
}

void Song::addAttachment( Attachment* attachment )
{
    assert( attachment->song() == this );
    int index = m_attachments.length();
    insertAttachment( attachment, index );

}

void Song::insertAttachment(Attachment *attachment, int index)
{
    m_attachments.insert( index, attachment );
    connectAttachment( attachment );
    emit attachmentAdded(index);
}

void Song::connectAttachment(Attachment *attachment)
{
    connect( attachment, &Attachment::attachmentRenamed, [this](QString name)
    {
        emit attachmentRenamed( m_songDatabase->songs().indexOf(this), name );
    });
}





