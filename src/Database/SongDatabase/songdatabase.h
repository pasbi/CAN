#ifndef SONGDATABASE_H
#define SONGDATABASE_H

#include "Database/database.h"
#include "song.h"

// SongID is the position of a song in a song database.
// it may change during runtime. Use SongID for saving and restoring and Song* otherwise.
typedef qint64 SongID;
class SongDatabaseSortProxy;
class CellEditor;
class SongDatabase : public Database<Song>
{

    Q_OBJECT
public:
    SongDatabase(Project* project);

    /////////////////////////////////////////////////
    ////
    ///  QAbstractTableModel
    //
    /////////////////////////////////////////////////
public:
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    // Drag'n'Drop
    // we only allow songs to be dragged (LinkAction). No redordering.
    Qt::DropActions supportedDragActions() const;

    QString fileNameBase() const { return "song"; }
    QString itemName(int n) const;

    QStringList peoples() const;

private:
    QStringList m_peoples;

signals:
    void attachmentAdded(int);
    void attachmentRemoved(int);
    void attachmentRenamed(int, QString);
    void songRemoved(int);

    friend class SongDatabaseSortProxy;

protected:
    void serialize(QDataStream &out) const;
    void deserialize(QDataStream &in);

};
#endif // SONGDATABASE_H
