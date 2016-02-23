#include "eventmergewidget.h"
#include <QHBoxLayout>

EventMergeWidget::EventMergeWidget(QWidget *parent) :
    QWidget(parent),
    m_mergeTreeView(new MergeTreeView(this)),
    m_eventDatabaseMerger(nullptr)
{
    setLayout(new QHBoxLayout(this));
    layout()->addWidget(m_mergeTreeView);
}

EventMergeWidget::~EventMergeWidget()
{
    delete m_eventDatabaseMerger;
    m_eventDatabaseMerger = nullptr;
}

void EventMergeWidget::setDatabase(Database<Event> *master, Database<Event> *slave)
{
    Q_ASSERT(!m_eventDatabaseMerger);
    m_eventDatabaseMerger = new DatabaseMerger<Event>(master, slave);
    m_mergeTreeView->setRootItem(m_eventDatabaseMerger->createItem());
}
