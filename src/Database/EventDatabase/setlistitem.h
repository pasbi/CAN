#ifndef SETLISTITEM_H
#define SETLISTITEM_H

#include <QJsonObject>
#include "Database/SongDatabase/song.h"

class SetlistItem
{
public:
    enum Type { SongType, LabelType };
    SetlistItem( const QString & label ) :
        m_type( LabelType ),
        m_label( label )
    {    }

    SetlistItem( ) :
        m_type( LabelType ),
        m_label( QObject::tr("Unnamed") )
    {    }

    SetlistItem( const Song* song ) :
        m_type( SongType ),
        m_song( song )
    {    }

    QString label() const;
    QString description() const { return label(); }
    bool setLabel( const QString label );
    const Song* song() const { return m_song; }
    Type type() const { return m_type; }
    QJsonObject toJson() const;
    static SetlistItem* fromJson( const QJsonObject & object );
    SetlistItem* copy() const;

private:
    Type m_type;
    const Song* m_song = NULL;
    QString m_label;

    friend QDataStream& operator << (QDataStream& out, const SetlistItem* item );
    friend QDataStream& operator >> (QDataStream& in,  SetlistItem* &item );
};

//TODO I think stream operators are not longer required
QDataStream& operator << (QDataStream& out, const SetlistItem* item );
QDataStream& operator >> (QDataStream& in,  SetlistItem* &item );

#endif // SETLISTITEM_H
