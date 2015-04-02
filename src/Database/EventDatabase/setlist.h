#ifndef SETLIST_H
#define SETLIST_H

#include <QAbstractListModel>
#include "Database/SongDatabase/song.h"

class SetlistItem
{
public:
    enum Type { SongType, LabelType };
    SetlistItem( const QString & label ) :
        m_type( LabelType ),
        m_label( label )
    {    }

    SetlistItem( const Song* song ) :
        m_type( SongType ),
        m_song( song )
    {    }

    QString label() const;
    bool setLabel( const QString label );
    const Song* song() const { return m_song; }
    Type type() const { return m_type; }

private:
    Type m_type;
    const Song* m_song = NULL;
    QString m_label;


};

class Setlist : public QAbstractListModel
{
    Q_OBJECT
public:
    Setlist(QObject* parent = NULL);
    ~Setlist() {}

    void insertItem( int position, SetlistItem* item );
    void appendItem( SetlistItem* item );
    void removeItem( SetlistItem* item );
    int rowCount(const QModelIndex &parent) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    SetlistItem* itemAt(const QModelIndex &index ) const;
    int indexOf(const SetlistItem *item ) const;

    void notifyDataChanged(const QModelIndex &index);
    void notifyDataChanged(const QModelIndex & start, const QModelIndex & end);
    void notifyDataChanged(const SetlistItem *item);

private:
    bool insertRows(int row, int count, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent);

    friend class SetlistEditDataCommand;
    bool setData_(const QModelIndex &index, const QVariant &value, int role);

    QList<SetlistItem*> m_items;
    QList<SetlistItem*> m_tmpItemBuffer;
};

#endif // SETLIST_H
