#ifndef SETLISTITEM_H
#define SETLISTITEM_H

#include <QJsonObject>
#include "Database/databaseitem.h"

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

    QString label() const;
    QString description() const { return label(); }
    bool setLabel( const QString label );
    const Song* song() const { return m_song; }
    Type type() const { return m_type; }
    QJsonObject toJsonObject() const;
    bool restoreFromJsonObject(const QJsonObject &object);
    QStringList textAttributes() const;

    static QString labelSong(const Song* song);
    void setSong(const Song *song);

    void serialize(QDataStream &out) const;
    void deserialize(QDataStream &in);

private:
    Type m_type;
    const Song* m_song = nullptr;
    QString m_label;

    QMetaObject::Connection m_updateSongLabelConnection;
};

Q_DECLARE_METATYPE(SetlistItem*)

#endif // SETLISTITEM_H
