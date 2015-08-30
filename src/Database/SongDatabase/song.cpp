#include "song.h"
#include "songdatabase.h"
#include "global.h"
#include <QJsonDocument>

Song::Song(SongDatabase* database) :
    m_songDatabase(database),
    m_creationDateTime(QDateTime::currentDateTime())
{
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

    // get program
    if (json.contains("program") && json["program"].isObject())
    {
        QJsonObject program = json["program"].toObject();
        if (        checkJsonObject( program, "bank",    QJsonValue::Double )
                &&  checkJsonObject( program, "valid",   QJsonValue::Bool )
                &&  checkJsonObject( program, "program", QJsonValue::Double )
                    )
        {
            m_program.bank =    program["bank"].toInt();
            m_program.program = program["program"].toInt();
            m_program.valid =   program["valid"].toBool();
        }
    }

    QJsonArray attributes = json["attributes"].toArray();
    m_title = attributes[0].toString();
    m_artist = attributes[1].toString();
    m_creationDateTime = QDateTime::fromString(attributes[2].toString(), DATE_TIME_FORMAT);
    m_duration = QTime::fromString(attributes[3].toString(), DATE_TIME_FORMAT);

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

    // attributes
    //TODO simplify format
    QJsonArray attributes;
    attributes.append(m_title);
    attributes.append(m_artist);
    attributes.append(m_creationDateTime.toString(DATE_TIME_FORMAT));
    attributes.append(m_duration.toString(DATE_TIME_FORMAT));
    json.insert("attributes", attributes);

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

    // program
    QJsonObject program;
    program["bank"] =       m_program.bank;
    program["program"] =    m_program.program;
    program["valid"] =      m_program.valid;
    json["program"] = program;



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





