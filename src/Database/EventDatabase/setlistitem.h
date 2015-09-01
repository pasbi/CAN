#ifndef SETLISTITEM_H
#define SETLISTITEM_H

#include <QJsonObject>
#include <QObject>

class Song;
class SetlistItem
{
public:
    enum Type { SongType, LabelType };
    SetlistItem( const QString & label );
    SetlistItem( );
    SetlistItem( const Song* song );

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
};

#endif // SETLISTITEM_H
