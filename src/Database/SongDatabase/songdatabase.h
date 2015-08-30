#ifndef SONGDATABASE_H
#define SONGDATABASE_H

#include "Database/database.h"
#include "song.h"
#include <QBitArray>

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
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data( const int row, const int column, const int role);

    SongID songID( const Song* song ) const;
    Song* song(const QString &id ) const;

    QModelIndex indexOfSong( const Song* song ) const;

    // Drag'n'Drop
    // we only allow songs to be dragged (LinkAction). No redordering.
    Qt::DropActions supportedDragActions() const;
    Qt::DropActions supportedDropActions() const;
    QMimeData* mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);


private:
    mutable QList<Song*> m_tmpSongBuffer; // Songs that was just removed or are about to be inserted.
    friend class SongDatabaseNewSongCommand;
    friend class SongDatabaseRemoveSongCommand;
    friend class SongDatabaseMoveSongCommand;

    /**
     * @brief appendSong this takes ownership of song
     * @param song
     */
    void appendSong(Song* song);
    void insertSong(Song* song, const int index);
    bool insertRows(int row, int count, const QModelIndex &parent);

    /**
     * @brief removeSong ownership is transfered to calling object
     * @param song
     * @return returns the index of the removed song.
     */
    int removeSong(Song *song);
    bool removeRows(int row, int count, const QModelIndex &parent);


public:
    bool columnIsVisible(int i) const { return m_columnIsVisible[i]; }
    void setColumnVisible(int i, bool visible) { m_columnIsVisible[i] = visible; }
private:
    QBitArray m_columnIsVisible;




    /////////////////////////////////////////////////
    ////
    ///  Converting to and from JsonObject
    //
    /////////////////////////////////////////////////
public:
    bool restoreFromJsonObject(const QJsonObject & object);
    QJsonObject toJsonObject() const;
    bool loadFrom(const QString &path);
    QList<File> getFiles() const;
public slots:
    void reset();

signals:
    void attachmentAdded(int);
    void attachmentRemoved(int);
    void attachmentRenamed(int, QString);
    void songAdded(int, Song*);
    void songRemoved(int);

    friend class SongDatabaseSortProxy;

};

#endif // SONGDATABASE_H
