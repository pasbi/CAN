#ifndef SONGDATABASE_H
#define SONGDATABASE_H

#include "Database/database.h"
#include <QAbstractTableModel>
#include "song.h"

class SongDatabase : public Database, public QAbstractTableModel
{

public:
    SongDatabase();



    /////////////////////////////////////////////////
    ////
    ///  QAbstractTableModel
    //
    /////////////////////////////////////////////////
public:
    int rowCount(const QModelIndex &parent) const;
    int rowCount() const { return rowCount(QModelIndex()); }
    int columnCount(const QModelIndex &parent) const;
    int columnCount() const { return columnCount(QModelIndex()); }
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
private:
    mutable QList<Song*> m_tmpSongBuffer; // Songs that was just removed or are about to be inserted.
    friend class SongDatabaseSetDataCommand;
    friend class SongDatabaseNewSongCommand;
    friend class SongDatabaseRemoveSongCommand;
    friend class SongDatabaseNewAttributeCommand;
    friend class SongDatabaseRemoveAttributeCommand;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    /**
     * @brief appendSong this takes ownership of song
     * @param song
     */
    void appendSong(Song* song);
    bool insertRows(int row, int count, const QModelIndex &parent);

    /**
     * @brief removeSong ownership is transfered to calling object
     * @param song
     */
    void removeSong(Song *song);
    bool removeRows(int row, int count, const QModelIndex &parent);


    bool insertColumns(int column, int count, const QModelIndex &parent);
    bool removeColumns(int column, int count, const QModelIndex &parent);



private:
    QList<Song*> m_songs;
};

#endif // SONGDATABASE_H
