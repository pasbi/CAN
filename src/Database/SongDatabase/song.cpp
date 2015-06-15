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

    setAttribute(2, QDateTime::currentDateTime());
}

Song::~Song()
{
    qDeleteAll( m_attachments );
}


// QVariant can not be saved as QJsonValue without losing its type.
// codec QVariant as string to prevent this.
QString encodeAttribute( const QVariant& variant )
{
    return QString("%1,%2").arg(variant.type()).arg(variant.toString());
}

QVariant decodeAttribute( const QString& string )
{
    QStringList tokens = string.split( "," );
    if (tokens.isEmpty() )
    {
        return QVariant();
    }
    else
    {
        QVariant::Type type = (QVariant::Type) tokens.first().toInt();
        QString value = string.mid( tokens.first().length() + 1 );

        switch (type)
        {
        case QVariant::String:
            return QVariant::fromValue( QString(value) );
        case QVariant::DateTime:
            return QVariant::fromValue( QDateTime::fromString(value, Qt::ISODate) );
        case QVariant::Int:
            return QVariant::fromValue( value.toInt() );
        default:
            qWarning() << "Type " << type << " is currently not supported.";
            return QVariant::fromValue( value );
        }
    }
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

    if (!checkJsonObject( json, "id", QJsonValue::String))
    {
        qWarning() << "expected id. create one: " << randomID();
    }
    else
    {
        m_randomID = json["id"].toString();
    }

    QJsonArray attributes = json["attributes"].toArray();
    m_attributes.clear();
    m_attributes.reserve(attributes.size());
    for (const QJsonValue & val : attributes)
    {
        m_attributes.append( decodeAttribute( val.toString() ) );
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
        attributes.append( encodeAttribute( v ));
    }
    json.insert("attributes", attributes);

    QJsonArray attachments;
    for (const Attachment* a : m_attachments)
    {
        QJsonObject o = a->toJsonObject();
        attachments.append( o );
    }
    json.insert("attachments", attachments);
    json["id"] = randomID();



    return json;
}


QString Song::title() const
{
    assert( m_attributes.size() >= 1 );
    return attribute(0).toString();
}

QString Song::artist() const
{
    assert( m_attributes.size() >= 1 );
    return attribute(1).toString();
}

QDateTime Song::creationTime() const
{
    assert( m_attributes.size() >= 1 );
    return attribute(2).toDateTime();
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
    // copied songs may have inapropriate attribute length.

    while ( index >= attributes().length() )
    {
        qWarning() << "Attribute " << index << " of " << title() << " does not exist. Create one.";
        m_attributes.append( QVariant() );
    }

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
    int index = m_attachments.length();
    insertAttachment( attachment, index );
}

void Song::insertAttachment(Attachment *attachment, int index)
{
    assert( attachment->song() == this );
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

Song* Song::copy() const
{
    QJsonObject json = toJsonObject();
    Song* song = new Song( database() );
    song->restoreFromJsonObject( json );
    return song;
}





