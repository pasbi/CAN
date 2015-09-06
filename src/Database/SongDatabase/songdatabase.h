#ifndef SONGDATABASE_H
#define SONGDATABASE_H

#include <QBitArray>

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
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    SongID songID( const Song* song ) const;
    Song* song(const QString &id ) const;

    // Drag'n'Drop
    // we only allow songs to be dragged (LinkAction). No redordering.
    Qt::DropActions supportedDragActions() const;
    void reset();

    QString fileNameBase() const { return "song"; }


    /////////////////////////////////////////////////
    ////
    ///  Converting to and from JsonObject
    //
    /////////////////////////////////////////////////
public:
    QJsonObject toJsonObject() const;
    bool restoreFromJsonObject(const QJsonObject &object);

signals:
    void attachmentAdded(int);
    void attachmentRemoved(int);
    void attachmentRenamed(int, QString);
    void songRemoved(int);

    friend class SongDatabaseSortProxy;

};

#endif // SONGDATABASE_H
