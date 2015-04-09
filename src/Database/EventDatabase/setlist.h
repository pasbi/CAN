#ifndef SETLIST_H
#define SETLIST_H

#include <QAbstractListModel>
#include "Database/SongDatabase/song.h"
#include <QDataStream>

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
    bool setLabel( const QString label );
    const Song* song() const { return m_song; }
    Type type() const { return m_type; }
    QJsonObject toJson() const;
    static SetlistItem* fromJson( const QJsonObject & object );

private:
    Type m_type;
    const Song* m_song = NULL;
    QString m_label;

    friend QDataStream& operator << (QDataStream& out, const SetlistItem* item );
    friend QDataStream& operator >> (QDataStream& in,  SetlistItem* &item );
};

QDataStream& operator << (QDataStream& out, const SetlistItem* item );
QDataStream& operator >> (QDataStream& in,  SetlistItem* &item );

class Setlist : public QAbstractListModel
{
    Q_OBJECT
public:
    Setlist(QObject* parent = NULL);
    ~Setlist() {}

    void insertItem( int position, SetlistItem* item );
    void appendItem( SetlistItem* item );
    void removeItem( SetlistItem* item );
    void moveItem( int from, int to );
    int rowCount(const QModelIndex &parent) const;
    int rowCount() const { return rowCount( QModelIndex() ); }
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    SetlistItem* itemAt(const QModelIndex &index ) const;
    int indexOf(const SetlistItem *item ) const;

    void notifyDataChanged(const QModelIndex &index);
    void notifyDataChanged(const QModelIndex & start, const QModelIndex & end);
    void notifyDataChanged(const SetlistItem *item);

    Qt::DropActions supportedDragActions() const;
    QMimeData* mimeData(const QModelIndexList &indexes) const;

    QJsonArray toJson() const;
    bool fromJson(const QJsonArray & array );

    void removeDraggedItems();

    QList<const Song *> songs() const;


private:
    bool insertRows(int row, int count, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent);

    friend class SetlistEditDataCommand;
    bool setData_(const QModelIndex &index, const QVariant &value, int role);

    QList<SetlistItem*> m_items;
    mutable QList<SetlistItem*> m_tmpItemBuffer;    // temp buffer for inserting items
    mutable QList<SetlistItem*> m_draggedItems;     // temp buffer for remove dragged items
};

#endif // SETLIST_H
