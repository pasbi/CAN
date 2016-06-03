#include "setlistitem.h"
#include "Project/project.h"
#include "Database/SongDatabase/songdatabase.h"
#include "Database/EventDatabase/setlist.h"
#include "Database/EventDatabase/event.h"
#include "jarowinkler.h"

SetlistItem::SetlistItem(Database<SetlistItem> *setlist, const QString &label, const Song *song) :
    DatabaseItem(setlist)
{
    Q_ASSERT( label.isEmpty() != (song == nullptr) );
    Type type = label.isEmpty() ? SongType : LabelType;

    addAttributeKey("type");
    addAttributeKey("label");
    addAttributeKey("song");


    setAttribute("type", QVariant::fromValue(type));
    setAttribute("label", label);
    setAttribute("song", QVariant::fromValue<const Song*>(song));
}

SetlistItem::SetlistItem(Database<SetlistItem> *setlist, const QString & label ) :
    SetlistItem(setlist, label, nullptr)
{
}

SetlistItem::SetlistItem(Database<SetlistItem>* setlist ) :
    SetlistItem(setlist, QObject::tr("Unnamed"), nullptr )
{
}

SetlistItem::SetlistItem(Database<SetlistItem>* setlist, const Song* song ) :
    SetlistItem(setlist, QString(), song)
{
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

void SetlistItem::serialize(QDataStream& out) const
{
    DatabaseItem::serialize(out);
    if (attribute("type").value<Type>() == SongType)
    {
        out << static_cast<qint32>(database()->project()->songDatabase()->identifyItem(attribute("song").value<const Song*>()));
    }
}

void SetlistItem::deserialize(QDataStream& in)
{
    DatabaseItem::deserialize(in);
    if (attribute("type").value<Type>() == SongType)
    {
        qint32 id;
        in >> id;
        //we cannot guarantee that the project is app().project().
        setSong( database()->project()->songDatabase()->retrieveItem(id) );
    }
}

DatabaseItemBase::Ratio SetlistItem::similarity(const DatabaseItemBase *other) const
{
    SIMILARITY_BEGIN_CHECK(SetlistItem);

    Type thisType = attribute("type").value<Type>();
    Type otherType = otherSetlistItem->attribute("type").value<Type>();

    if (thisType == otherType)
    {
        if (thisType == LabelType)
        {
            return Ratio(jaro_winkler_distance(label(), other->label()), 1.0);
        }
        else if (thisType == SongType)
        {
            if (attribute("song").value<const Song*>() == other->attribute("song").value<const Song*>())
            {
                return Ratio(1, 1);
            }
        }
        else
        {
            Q_UNREACHABLE();
        }
    }
    return Ratio(0, 1);

}

DEFINE_ENUM_STREAM_OPERATORS(SetlistItem::Type)
