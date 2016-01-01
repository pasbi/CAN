#include "freezetableview.h"
#include <QHeaderView> //TODO
#include <QScrollBar>

FreezeTableView::FreezeTableView(QWidget *parent) :
    Super(parent)
{
    m_frozenTableView = new QTableView(this);
    m_frozenTableView->setFocusPolicy(Qt::NoFocus);
    m_frozenTableView->verticalHeader()->hide();

    m_frozenTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    viewport()->stackUnder(m_frozenTableView);

    m_frozenTableView->setColumnWidth(0, columnWidth(0));
    m_frozenTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_frozenTableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_frozenTableView->setStyleSheet("QTableView { border: none; }");

    updateFrozenTableGeometry();

    setHorizontalScrollMode(ScrollPerPixel);
    setVerticalScrollMode(ScrollPerPixel);
    m_frozenTableView->setHorizontalScrollMode(ScrollPerPixel);
    m_frozenTableView->setVerticalScrollMode(ScrollPerPixel);
    m_frozenTableView->setAlternatingRowColors(true);
    m_frozenTableView->horizontalHeader()->resize(width(), 100);

    m_frozenTableView->hide();

    connect(horizontalHeader(), SIGNAL(sectionResized(int,int,int)), this, SLOT(updateSectionWidth(int, int, int)));
    connect(verticalHeader(), SIGNAL(sectionResized(int,int,int)), this, SLOT(updateSectionHeight(int,int,int)));
    connect(m_frozenTableView->verticalScrollBar(), SIGNAL(valueChanged(int)), verticalScrollBar(), SLOT(setValue(int)));
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), m_frozenTableView->verticalScrollBar(), SLOT(setValue(int)));
    connect(m_frozenTableView->horizontalScrollBar(), &QScrollBar::valueChanged, [this]()
    {
        m_frozenTableView->horizontalScrollBar()->setValue(0);
    });

    horizontalHeader()->setFixedHeight(20);
    m_frozenTableView->horizontalHeader()->setFixedHeight(20);

    m_frozenTableView->horizontalHeader()->setSortIndicatorShown( true );
    m_frozenTableView->horizontalHeader()->setSectionsClickable( true );
    m_frozenTableView->setSortingEnabled( true );
    connect(m_frozenTableView->horizontalHeader(), &QHeaderView::sortIndicatorChanged, [this]()
    {
        horizontalHeader()->setSortIndicator(-1, Qt::DescendingOrder);
    });

    connect(horizontalHeader(), &QHeaderView::sortIndicatorChanged, [this]()
    {
        m_frozenTableView->horizontalHeader()->setSortIndicator(-1, Qt::DescendingOrder);
    });
}

FreezeTableView::~FreezeTableView()
{

}

void FreezeTableView::scrollTo(const QModelIndex &index, ScrollHint hint)
{
    if (index.column() > 0)
    {
        QTableView::scrollTo(index, hint);
    }
}

void FreezeTableView::updateSectionWidth(int logicalIndex, int oldSize, int newSize)
{
    Q_UNUSED(oldSize);
    if (logicalIndex == 0)
    {
        m_frozenTableView->setColumnWidth(0, newSize);
        updateFrozenTableGeometry();
    }
}

void FreezeTableView::updateSectionHeight(int logicalIndex, int oldSize, int newSize)
{
    Q_UNUSED(oldSize);
    m_frozenTableView->setRowHeight(logicalIndex, newSize);
}

void FreezeTableView::setModel(QAbstractItemModel *model)
{
    QTableView::setModel(model);
    m_frozenTableView->setModel(model);
    m_frozenTableView->setSelectionModel(selectionModel());
}

void FreezeTableView::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);
    updateFrozenTableGeometry();
}

QModelIndex FreezeTableView::moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
    QModelIndex current = QTableView::moveCursor(cursorAction, modifiers);
    if (cursorAction == MoveLeft && current.column() > 0 && visualRect(current).topLeft().x() < m_frozenTableView->columnWidth(0) )
    {
        const int newValue = horizontalScrollBar()->value() + visualRect(current).topLeft().x() - m_frozenTableView->columnWidth(0);
        horizontalScrollBar()->setValue(newValue);
    }
    return current;
}

void FreezeTableView::updateFrozenTableGeometry()
{
    m_frozenTableView->setGeometry( verticalHeader()->width() + frameWidth(),
                                    frameWidth(),
                                    columnWidth(0),
                                    viewport()->height() + horizontalHeader()->height());
}

void FreezeTableView::freezeFirstColumn(bool freeze)
{
    m_frozenTableView->setVisible(freeze);
}

void FreezeTableView::setSelectionMode(SelectionMode mode)
{
    m_frozenTableView->setSelectionMode(mode);
    Super::setSelectionMode(mode);
}

void FreezeTableView::setSelectionBehavior(SelectionBehavior behavior)
{
    m_frozenTableView->setSelectionBehavior(behavior);
    Super::setSelectionBehavior(behavior);
}
