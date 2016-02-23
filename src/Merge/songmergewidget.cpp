#include "songmergewidget.h"
#include <QHBoxLayout>

SongMergeWidget::SongMergeWidget(QWidget *parent) :
    QWidget(parent),
    m_mergeTreeView(new MergeTreeView(this)),
    m_songDatabaseMerger(nullptr)
{
    setLayout(new QHBoxLayout(this));
    layout()->addWidget(m_mergeTreeView);
}

SongMergeWidget::~SongMergeWidget()
{
    delete m_songDatabaseMerger;
    m_songDatabaseMerger = nullptr;
}

void SongMergeWidget::setDatabase(Database<Song> *master, Database<Song> *slave)
{
    Q_ASSERT(!m_songDatabaseMerger);
    m_songDatabaseMerger = new DatabaseMerger<Song>(master, slave);
    m_mergeTreeView->setRootItem(m_songDatabaseMerger->createItem());
}
