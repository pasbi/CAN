#ifndef SETLISTITEM_H
#define SETLISTITEM_H

#include <QJsonObject>
#include "Database/databaseitem.h"
#include "Database/SongDatabase/song.h"

class Song;
class Setlist;
template<typename T> class Database;
class SetlistItem : public DatabaseItem<SetlistItem>
{
    Q_OBJECT
public:
    enum Type { SongType, LabelType };
    SetlistItem( Database<SetlistItem>* setlist, const QString & label );
    SetlistItem( Database<SetlistItem>* setlist );
    SetlistItem( Database<SetlistItem>* setlist, const Song* song );
    virtual ~SetlistItem();


    void serialize(QDataStream &out) const;
    void deserialize(QDataStream &in);

    void setSong(const Song* song);

    QString attributeDisplay(const QString &key) const;

    static const QStringList ATTRIBUTE_KEYS;
    QStringList skipSerializeAttributes() const;

    QString label() const;

private:
    QMetaObject::Connection m_updateSongLabelConnection;
};

Q_DECLARE_METATYPE(SetlistItem*)
Q_DECLARE_METATYPE(SetlistItem::Type)
REGISTER_META_TYPE_STREAM_OPERATORS(SetlistItem::Type, SetlistItemType)
DECLARE_ENUM_STREAM_OPERATORS(SetlistItem::Type)

#endif // SETLISTITEM_H
