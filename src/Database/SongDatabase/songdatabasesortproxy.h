#ifndef SONGDATABASESORTPROXY_H
#define SONGDATABASESORTPROXY_H

#include "Database/databasesortproxy.h"
#include "songdatabase.h"

class SongDatabaseSortProxy : public DatabaseSortProxy<Song>
{
    typedef DatabaseSortProxy<Song> Super;
    Q_OBJECT
public:
    explicit SongDatabaseSortProxy(QObject* parent = 0);
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

    void setHideInactives(bool hide);
    void setShowNormals(bool hide);
    void setShowAcoustics(bool hide);

private:
    bool m_hideInactives;
    bool m_showNormals;
    bool m_showAcoustics;
    bool acceptSong(const Song* song) const;
};

#endif // SONGDATABASESORTPROXY_H

