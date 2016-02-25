#include "eventmergewidget.h"
#include <QHBoxLayout>

EventMergeWidget::EventMergeWidget(QWidget *parent) :
    QWidget(parent),
    m_mergeListView(new MergeListView(this)),
    m_eventDatabaseMerger(nullptr)
{
    setLayout(new QHBoxLayout(this));
    layout()->addWidget(m_mergeListView);
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
    m_mergeListView->setItems(m_eventDatabaseMerger->createItems());
}
