//#include "songmergewidget.h"
//#include <QHBoxLayout>
//#include "mergelistwidget.h"
//#include "combinesongsdialog.h"

//SongMergeWidget::SongMergeWidget(QWidget *parent) :
//    QWidget(parent),
//    m_mergeListView(new MergeListView(this)),
//    m_songDatabaseMerger(nullptr)
//{
//    setLayout(new QHBoxLayout(this));
//    layout()->addWidget(m_mergeListView);

//    connect(mergeListWidget(), SIGNAL(combineItemDialogRequest(MergeItemBase&)), this, SLOT(showCombineSongsDialog(MergeItemBase&)));
//}

//SongMergeWidget::~SongMergeWidget()
//{
//    delete m_songDatabaseMerger;
//    m_songDatabaseMerger = nullptr;
//}

//void SongMergeWidget::setDatabase(Database<Song> *master, Database<Song> *slave)
//{
//    Q_ASSERT(!m_songDatabaseMerger);
//    m_songDatabaseMerger = new DatabaseMerger<Song>(master, slave);
//    m_mergeListView->setItems(m_songDatabaseMerger->createItems());
//}

//void SongMergeWidget::showCombineSongsDialog(MergeItemBase *mergeItemBase)
//{
//    const Song* masterSong = static_cast<const Song*>(mergeItemBase->masterPointer());
//    const Song* slaveSong = static_cast<const Song*>(mergeItemBase->slavePointer());
//    CombineSongsDialog dialog(masterSong, slaveSong, this);
//    if (dialog.exec() == QDialog::Accepted)
//    {
//        mergeItemBase->deleteCombinationObject();
//        mergeItemBase->setCombinationPointer(dialog.combination());
//    }
//}
