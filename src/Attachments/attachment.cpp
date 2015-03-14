#include "attachment.h"
#include "Database/SongDatabase/song.h"

Attachment::Attachment()
{
}

void Attachment::setSong(Song *song)
{
    // set song, but do never overwrite.
    assert( !m_song );

    m_song = song;
}

void Attachment::setName(const QString &name)
{
    if (name != m_name)
    {
        m_name = name;
        emit attachmentRenamed( m_name );
    }
}

void Attachment::makeNameUnique()
{
    assert( m_song );

    const QStringList names = m_song->attachmentNames();

    QString newName = name();
    int i = 1;
    while ( names.contains( newName ) )
    {
        newName = QString("%1 %2").arg(name()).arg(i++);
    }

    setName( newName );
}

bool Attachment::create(const QJsonObject &object, Attachment *&attachment)
{
    checkJsonObject( object, "classname", QJsonValue::String );

    QString classname = object.value("classname").toString();
    if (Creatable::category(classname) != "Attachment")
    {
        qWarning() << "Cannot create attachment " << classname << ".";
        return false;
    }

    CREATE( classname, attachment );

    return attachment->restoreFromJsonObject( object );
}


QJsonObject Attachment::toJsonObject() const
{
    QJsonObject object = Taggable::toJsonObject();

    object.insert("classname", classname());

    return object;
}





















