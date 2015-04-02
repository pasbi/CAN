#ifndef SETLIST_H
#define SETLIST_H

#include <QAbstractListModel>
#include "Database/SongDatabase/song.h"

class SetlistItem
{
public:
    SetlistItem( const QString & label ) :
        m_type( LabelType ),
        m_label( label )
    {    }

    SetlistItem( const Song* song ) :
        m_type( SongType ),
        m_song( song )
    {    }

    QString label() const;
    const Song* song() const { return m_song; }

private:
    enum Type { SongType, LabelType };
    Type m_type;
    const Song* m_song = NULL;
    QString m_label;


};

class Setlist : public QAbstractListModel
{
public:
    Setlist(QObject* parent = NULL);

    void insertItem( int position, SetlistItem* item );
    void appendItem( SetlistItem* item );
    void removeItem( SetlistItem* item );
    int rowCount(const QModelIndex &parent) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;

private:
    bool insertRows(int row, int count, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent);

    QList<SetlistItem*> m_items;
    QList<SetlistItem*> m_tmpItemBuffer;
};

#endif // SETLIST_H
