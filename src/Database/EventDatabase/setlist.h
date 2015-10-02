#ifndef SETLIST_H
#define SETLIST_H

#include "Database/database.h"
#include "global.h"

class Song;
class Event;
class SetlistItem;
class Setlist : public Database<SetlistItem>
{
    Q_OBJECT
public:
    Setlist( Event* event );
    virtual ~Setlist();

    int columnCount( const QModelIndex& parent = QModelIndex() ) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    Qt::DropActions supportedDragActions() const;
    Qt::DropActions supportedDropActions() const;
    QMimeData* mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    QStringList mimeTypes() const;

    QList<const Song *> songs() const;
    Event const* event() const;

    QString fileNameBase() const { return "SetlistItem"; }


signals:
    void selectionRequest(QModelIndexList);


private:
    QList<void*> viewableAttachments( const QModelIndex& index ) const;    // returns a list of viewable attachments
    Event* m_event = nullptr;

    // helper to keep dropMimeData clear.
    bool dropSongs(const DatabaseMimeData<Song>*        songData,    int targetRow, QModelIndexList& indexes);
    void moveItems(const DatabaseMimeData<SetlistItem>* setlistData, int targetRow, QModelIndexList &indexes);
    void copyItems(const DatabaseMimeData<SetlistItem>* setlistData, int targetRow, QModelIndexList &indexes);


public slots:
    void setFilter(QString filter);
public:
    QString filter() const { return m_filter; }
private:
    QString m_filter = "";
};

#endif // SETLIST_H
