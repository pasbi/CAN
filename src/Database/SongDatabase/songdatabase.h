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

public:
    class AttributeKey : public QString
    {
    public:
        AttributeKey( const QString& string ) :
            QString( string ),
            visible( true )
        {}
        bool visible;
        QJsonObject toJson() const
        {
            QJsonObject o;
            o["key"] = *this;
            o["visible"] = visible;
            return o;
        }

        static AttributeKey fromJson( const QJsonObject& o )
        {
            checkJsonObject( o, "key", QJsonValue::String );
            checkJsonObject( o, "visible", QJsonValue::Bool );
            AttributeKey ak(o["key"].toString());
            ak.visible = o["visible"].toBool();
            return ak;
        }
    };

    Q_OBJECT
public:
    SongDatabase(Project* project);
    QStringList attributeKeys() const ;

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
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data( const int row, const int column, const int role);
    void moveRow( int sourceRow, int targetRow );


    static QString extractEditorType(const QString & key);
    static QString extractHeaderLabel(const QString & key);

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
    mutable QList<AttributeKey> m_tmpColumnNameBuffer;
    mutable QList<QVariantList> m_tmpColumnContentBuffer;
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

    void appendColumn(const AttributeKey &label);
    void insertColumn(const int section, const AttributeKey &label);
    void restoreColumn(const int section, const QString& label, const QVariantList &attributes);
    bool insertColumns(int column, int count, const QModelIndex &parent);
    bool removeColumns(int column, int count, const QModelIndex &parent);

    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);

    static bool isStandardAttribute(const int section);



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
    bool attributeVisible( int i );
    void setAttributeVisible( int i, bool visible );
    bool allowDeleteColumn(int i);
    bool fixColumnEditor(int i);

private:

    QList<AttributeKey>  m_attributeKeys;
    friend class SongDatabaseSortProxy;



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
    void reset(bool initialize = true);

private:
    QList<AttributeKey> m_attributeKeysToRestore;
    void initAttributes();



public:
    static void editorTypeAndHeaderLabel( const QString & encoding, QString & editorType, QString & attributeValue );


signals:
    void attachmentAdded(int);
    void attachmentRemoved(int);
    void attachmentRenamed(int, QString);
    void songAdded(int, Song*);
    void songRemoved(int);

    //initial columns
private:
    struct InitialColumn
    {
        QString caption;
        bool fixEditor;
    };
    static const QList<InitialColumn> INITIAL_COLUMNS;
    friend QList<InitialColumn> initColumns();


};

#endif // SONGDATABASE_H
