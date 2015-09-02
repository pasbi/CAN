#ifndef SETLISTITEM_H
#define SETLISTITEM_H

#include <QJsonObject>
#include "Database/databaseitem.h"

class Song;
class Setlist;
template<typename T> class Database;
class SetlistItem : public DatabaseItem<SetlistItem>
{
public:
    enum Type { SongType, LabelType };
    SetlistItem( Database<SetlistItem>* setlist, const QString & label );
    SetlistItem( Database<SetlistItem>* setlist );
    SetlistItem( Database<SetlistItem>* setlist, const Song* song );
    ~SetlistItem();

    QString label() const;
    QString description() const { return label(); }
    bool setLabel( const QString label );
    const Song* song() const { return m_song; }
    Type type() const { return m_type; }
    QJsonObject toJson() const;
    bool restoreFromJsonObject(const QJsonObject &object);

private:
    Type m_type;
    const Song* m_song = NULL;
    QString m_label;
};

#endif // SETLISTITEM_H
