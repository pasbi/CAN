#ifndef SONGMERGEWIDGET_H
#define SONGMERGEWIDGET_H

#include <QWidget>
#include "databasemerger.h"
#include "mergetreeview.h"


class Song;

class SongMergeWidget : public QWidget
{
public:
    explicit SongMergeWidget(QWidget* parent = nullptr);
    ~SongMergeWidget();
    void setDatabase(Database<Song> *master, Database<Song> *slave);
    const MergeTreeItem* rootItem() const { return m_mergeTreeView->root(); }

private:
    MergeTreeView* m_mergeTreeView;
    DatabaseMerger<Song>* m_songDatabaseMerger;
};

#endif // SONGMERGEWIDGET_H
