#ifndef SONGMERGEWIDGET_H
#define SONGMERGEWIDGET_H

#include <QWidget>
#include "databasemerger.h"
#include "mergelistview.h"


class Song;

class SongMergeWidget : public QWidget
{
public:
    explicit SongMergeWidget(QWidget* parent = nullptr);
    ~SongMergeWidget();
    void setDatabase(Database<Song> *master, Database<Song> *slave);
    QList<MergeItemBase> items() const { return m_mergeListView->items(); }

private:
    MergeListView* m_mergeListView;
    DatabaseMerger<Song>* m_songDatabaseMerger;
};

#endif // SONGMERGEWIDGET_H
