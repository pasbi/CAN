#ifndef SETLISTITEM_H
#define SETLISTITEM_H

#include <QJsonObject>

class Song;
class Setlist;
class SetlistItem
{
public:
    enum Type { SongType, LabelType };
    SetlistItem( Setlist* setlist, const QString & label );
    SetlistItem( Setlist* setlist );
    SetlistItem( Setlist* setlist, const Song* song );
    ~SetlistItem();

    QString label() const;
    QString description() const { return label(); }
    bool setLabel( const QString label );
    const Song* song() const { return m_song; }
    Type type() const { return m_type; }
    QJsonObject toJson() const;
    static SetlistItem* fromJson(const QJsonObject & object , Setlist *setlist);
    SetlistItem* copy() const;

private:
    Type m_type;
    const Song* m_song = NULL;
    QString m_label;
    Setlist* m_setlist;
};

#endif // SETLISTITEM_H
