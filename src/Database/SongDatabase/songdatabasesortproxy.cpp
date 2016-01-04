#include "songdatabasesortproxy.h"


SongDatabaseSortProxy::SongDatabaseSortProxy(QObject *parent) :
    Super(parent)
{
    // filter all columns
    setFilterKeyColumn(-1);
    setDynamicSortFilter( false );
    setFilterCaseSensitivity( Qt::CaseInsensitive );
}


bool SongDatabaseSortProxy::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    assert(!source_parent.isValid());
    if (acceptSong(sourceModel()->items()[source_row]))
    {
        return Super::filterAcceptsRow(source_row, source_parent);
    }
    else
    {
        return false;
    }
}

void SongDatabaseSortProxy::setHideInactives(bool hide)
{
    m_hideInactives = hide;
    invalidate();
}

bool SongDatabaseSortProxy::acceptSong(const Song *song) const
{
    if (m_hideInactives && song->state() == Song::Inactive)
    {
        return false;
    }
    else
    {
        return true;
    }
}
