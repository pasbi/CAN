#ifndef SONGDATABASE_H
#define SONGDATABASE_H

#include "Database/database.h"
#include <QAbstractTableModel>
#include "song.h"

// SongID is the position of a song in a song database.
// it may change during runtime. Use SongID for saving and restoring and Song* otherwise.
typedef qint64 SongID;
class SongDatabaseSortProxy;
class CellEditor;
class SongDatabase : public QAbstractTableModel, public Database
{
    Q_OBJECT
public:
    SongDatabase(Project* project);

    QList<Song*> songs() const { return m_songs; }
    QStringList attributeKeys() const { return m_attributeKeys; }

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
    void notifyDataChanged(const QModelIndex &index);
    void notifyDataChanged(const QModelIndex & start, const QModelIndex & end);
    void notifyDataChanged(const Song *song);
    Song* songAtIndex(const QModelIndex & index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data( const int row, const int column, const int role);
    Qt::DropActions supportedDragActions() const;
    QMimeData* mimeData(const QModelIndexList &indexes) const;
    void moveRow( int sourceRow, int targetRow );

    static QString extractEditorType(const QString & key);
    static QString extractHeaderLabel(const QString & key);

    SongID songID( const Song* song ) const;
    Song* song( SongID id ) const;

    QModelIndex indexOfSong( const Song* song ) const;

private:
    mutable QList<Song*> m_tmpSongBuffer; // Songs that was just removed or are about to be inserted.
    mutable QStringList m_tmpColumnNameBuffer;
    mutable QList<QVariantList> m_tmpColumnContentBuffer;
    friend class SongDatabaseSetDataCommand;
    friend class SongDatabaseNewSongCommand;
    friend class SongDatabaseRemoveSongCommand;
    friend class SongDatabaseNewAttributeCommand;
    friend class SongDatabaseRemoveAttributeCommand;
    friend class SongDatabaseRenameHeaderCommand;
    friend class SongDatabaseRemoveColumnCommand;
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
    bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild);

    void appendColumn(const QString & label);
    void insertColumn(const int section, const QString & label);
    void restoreColumn(const int section, const QString& label, const QVariantList &attributes);
    bool insertColumns(int column, int count, const QModelIndex &parent);
    bool removeColumns(int column, int count, const QModelIndex &parent);

    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);

    static bool isStandardAttribute(const int section);




private:
    QList<Song*> m_songs;


    /////////////////////////////////////////////////
    ////
    ///  Attribute Keys, like title, artist, year, etc.
    ///  basically, attributes can be addded, removed, renamed by user.
    ///  To ensure integrity and ability to show special delegates,
    ///  there are some standard attribute keys that cannot be removed.
    //
    /////////////////////////////////////////////////
public:
    QString editorType( const QModelIndex & index ) const;
private:
    QStringList  m_attributeKeys;



    /////////////////////////////////////////////////
    ////
    ///  Converting to and from JsonObject
    //
    /////////////////////////////////////////////////
public:
    bool restoreFromJsonObject(const QJsonObject & object);
    QJsonObject toJsonObject() const;
    bool saveTo(const QString &path) const;
    bool loadFrom(const QString &path);
public slots:
    void reset(bool initialize = true);
private:
    /**
     * @brief m_numSongsToRestore number of songs that will be restored. Use this only for loading issues.
     */
    int m_numSongsToRestore;
    QStringList m_attributeKeysToRestore;
    void initAttributes();





public:
    static const QString SONG_POINTERS_MIME_DATA_FORMAT;


signals:
    void attachmentAdded(int);
    void attachmentRemoved(int);
    void attachmentRenamed(int, QString);
    void songAdded(int, Song*);
    void songRemoved(int);

};

#endif // SONGDATABASE_H
