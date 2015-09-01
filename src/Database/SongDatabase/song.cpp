#include "song.h"
#include "songdatabase.h"
#include "global.h"
#include <QJsonDocument>

Song::Song(Database<Song> * database) :
    m_songDatabase(database),
    m_creationDateTime(QDateTime::currentDateTime())
{
    connect( this, SIGNAL(attachmentAdded(int)),   database, SIGNAL(attachmentAdded(int)  ));
    connect( this, SIGNAL(attachmentRemoved(int)), database, SIGNAL(attachmentRemoved(int)));
    connect( this, SIGNAL(attachmentRenamed(int, QString)), database, SIGNAL(attachmentRenamed(int,QString)));
}

Song::~Song()
{
    qDeleteAll( m_attachments );
}

bool Song::restoreFromJsonObject(const QJsonObject &json)
{
    if (!Taggable::restoreFromJsonObject(json))
    {
        return false;
    }


    if (json.contains("attributes")) // load legacy file format
    {
        const QJsonArray& array = json["attributes"].toArray();
        if (array.count() > 0)
        {
            m_title = array[0].toString();
        }
        if (array.count() > 1)
        {
            m_artist = array[1].toString();
        }
        if (array.count() > 2)
        {
            m_creationDateTime = QDateTime::fromString(array[2].toString(), DATE_TIME_FORMAT);
        }
        if (array.count() > 3)
        {
            m_duration = QTime::fromString(array[3].toString(), DATE_TIME_FORMAT);
        }
    }
    else    // load up-to-date file format
    {
        if (        !checkJsonObject(json, "title", QJsonValue::String)
                ||  !checkJsonObject(json, "artist", QJsonValue::String)
                ||  !checkJsonObject(json, "creationDateTime", QJsonValue::String)
                ||  !checkJsonObject(json, "duration", QJsonValue::String) )
        {
            return false;
        }

        m_title = json["title"].toString();
        m_artist = json["artist"].toString();
        m_creationDateTime = QDateTime::fromString(json["creationDateTime"].toString(), DATE_TIME_FORMAT);
        m_duration = QTime::fromString(json["duration"].toString(), DATE_TIME_FORMAT);
    }

    if (!checkJsonObject( json, "id", QJsonValue::String))
    {
        qWarning() << "expected id. create one: " << randomID();
    }
    else
    {
        m_randomID = json["id"].toString();
    }

    m_program.restoreFromJsonObject(json["program"].toObject());

    QJsonArray attachments = json["attachments"].toArray();
    m_attachments.clear();
    m_attachments.reserve(attachments.size());
    for (const QJsonValue & val : attachments)
    {
        Attachment* a = NULL;
        if (Attachment::create( val.toObject(), a, this))
        {
            m_attachments.append( a );
            connectAttachment(a);
        }
    }

    return true;
}

QJsonObject Song::toJsonObject() const
{
    QJsonObject json = Taggable::toJsonObject();

    json.insert("title", m_title);
    json.insert("artist", m_artist);
    json.insert("creationDateTime", m_creationDateTime.toString(DATE_TIME_FORMAT));
    json.insert("duration", m_duration.toString(DATE_TIME_FORMAT));

    // attachments
    QJsonArray attachments;
    for (const Attachment* a : m_attachments)
    {
        QJsonObject o = a->toJsonObject();
        attachments.append( o );
    }
    json.insert("attachments", attachments);

    // id
    json["id"] = randomID();

    json["program"] = m_program.toJsonObject();

    return json;
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
    connect( attachment, &Attachment::attachmentRenamed, [this, attachment](QString name)
    {
        emit attachmentRenamed( attachments().indexOf(attachment), name );
    });
}

Song* Song::copy() const
{
    QJsonObject json = toJsonObject();
    Song* song = new Song( database() );
    song->restoreFromJsonObject( json );
    return song;
}





