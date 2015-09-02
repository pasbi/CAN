#ifndef SETLIST_H
#define SETLIST_H

#include <QAbstractTableModel>

class Song;
class SetlistItem;
class Event;
class Setlist : public QAbstractTableModel
{
    Q_OBJECT
public:
    Setlist( Event* event );
    virtual ~Setlist() {}

    void insertItem(SetlistItem* item , int position = -1);
    void removeItem( SetlistItem* item );
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount( const QModelIndex& parent = QModelIndex() ) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    SetlistItem* itemAt(const QModelIndex &index ) const;
    QModelIndex indexOf(const SetlistItem *item ) const;
    QList<SetlistItem*> items() const { return m_items; }

    Qt::ItemFlags flags(const QModelIndex &index) const;
    Qt::DropActions supportedDragActions() const;
    Qt::DropActions supportedDropActions() const;
    QMimeData* mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    QStringList mimeTypes() const;

    void saveState();
    void restore();
    void acceptDrop();

    QJsonArray toJson() const;
    bool fromJson(const QJsonArray & array );

    QList<const Song *> songs() const;
    Event const* event() const;

signals:
    void selectionRequest(QModelIndexList);


private:
    bool insertRows(int row, int count, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent);
    bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild);

    friend class SetlistEditDataCommand;
    friend class SetlistMoveRowsCommand;
    bool setData_(const QModelIndex &index, const QVariant &value, int role);
    mutable QList<SetlistItem*> m_tmpItemBuffer;    // temp buffer for inserting items

    QList<void*> viewableAttachments( const QModelIndex& index ) const;    // returns a list of viewable attachments

    Event* m_event = NULL;

    QList<SetlistItem*> m_items;
    QList<SetlistItem*> m_savedState;
};

#endif // SETLIST_H
