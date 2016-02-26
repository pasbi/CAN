//#ifndef SONGMERGEWIDGET_H
//#define SONGMERGEWIDGET_H

//#include <QWidget>
//#include "databasemerger.h"
//#include "mergelistview.h"

//class MergeListWidget;
//class Song;

//class SongMergeWidget : public QWidget
//{
//    Q_OBJECT
//public:
//    explicit SongMergeWidget(QWidget* parent = nullptr);
//    ~SongMergeWidget();
//    void setDatabase(Database<Song> *master, Database<Song> *slave);
//    QList<MergeItemBase*> mergeItems() const { return m_mergeListView->mergeItems(); }
//    MergeListWidget* mergeListWidget() const { return m_mergeListView->mergeListWidget(); }

//public slots:
//    void showCombineSongsDialog(MergeItemBase* mergeItemBase);

//private:
//    MergeListView* m_mergeListView;
//    DatabaseMerger<Song>* m_songDatabaseMerger;
//};

//#endif // SONGMERGEWIDGET_H
