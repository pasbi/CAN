#include "setlistitem.h"
#include "Project/project.h"
#include "Database/SongDatabase/songdatabase.h"
#include "Database/EventDatabase/setlist.h"
#include "Database/EventDatabase/event.h"

const QStringList SetlistItem::ATTRIBUTE_KEYS = { "type", "label", "song" };

SetlistItem::SetlistItem(Database<SetlistItem> *setlist, const QString & label ) :
    DatabaseItem(ATTRIBUTE_KEYS, setlist)
{
    setAttribute("type", QVariant::fromValue(LabelType));
    setAttribute("label", label);
    setAttribute("song", QVariant::fromValue<const Song*>(nullptr));
}

SetlistItem::SetlistItem(Database<SetlistItem>* setlist ) :
    DatabaseItem(ATTRIBUTE_KEYS, setlist)
{
    setAttribute("type", QVariant::fromValue(LabelType));
    setAttribute("label", QObject::tr("Unnamed"));
    setAttribute("song", QVariant::fromValue<const Song*>(nullptr));
}

SetlistItem::SetlistItem(Database<SetlistItem>* setlist, const Song* song ) :
    DatabaseItem(ATTRIBUTE_KEYS, setlist)
{
    setAttribute("type", QVariant::fromValue(SongType));
    setAttribute("label", QString());
    setAttribute("song", QVariant::fromValue(song));
}

SetlistItem::~SetlistItem()
{
    if (attribute("song").value<const Song*>())
    {
        QObject::disconnect(m_updateSongLabelConnection);
    }
}

void SetlistItem::setSong(const Song *song)
{
    if (attribute("song").value<const Song*>())
    {
        QObject::disconnect(m_updateSongLabelConnection);
    }

    setAttribute("song", QVariant::fromValue(song));

    m_updateSongLabelConnection = QObject::connect(song, &Song::attributeChanged, [song, this]()
    {
        int row = database()->rowOf(this);
        if (row >= 0)
        {
            QModelIndex index = database()->index(row, 0);
            emit database()->dataChanged(index, index);
        }
    });
}

void SetlistItem::serialize(QDataStream &out) const
{
    DatabaseItem::serialize(out);
    if (attribute("type").value<Type>() == SongType)
    {
        out << static_cast<qint32>(app().project()->songDatabase()->identifyItem(attribute("song").value<const Song*>()));
    }
}

//TODO setlist item, song editing should be more user friendly (i.e. typing artist etc. instead of just title);


void SetlistItem::deserialize(QDataStream &in)
{
    DatabaseItem::deserialize(in); // see serialize
    if (attribute("type").value<Type>() == SongType)
    {
        qint32 id;
        in >> id;
        //we cannot guarantee that the project is app().project().
        setSong( database()->project()->songDatabase()->retrieveItem(id) );
    }
}

QString SetlistItem::attributeDisplay(const QString &key) const
{
    if (key == "song" || key == "type")
    {
        return "";
    }

    if (key == "label")
    {
        switch (attribute("type").value<Type>())
        {
        case SongType:
            return attribute("song").value<const Song*>()->label();
        case LabelType:
            return attribute("label").toString();
        }
    }

    Q_UNREACHABLE();
    return "";

}

QStringList SetlistItem::skipSerializeAttributes() const
{
    return QStringList({"song"});
}

QString SetlistItem::label() const
{
    return attributeDisplay("label");
}


DEFINE_ENUM_STREAM_OPERATORS(SetlistItem::Type)
