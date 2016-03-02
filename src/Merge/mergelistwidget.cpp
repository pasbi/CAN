#include "mergelistwidget.h"
#include "mergeitem.h"
#include <QMimeData>
#include "global.h"
#include <QDragEnterEvent>
#include <QPushButton>
#include <QMenu>
#include "mergelistwidgetitemwidget.h"
#include "merge.h"
#include "combinedatabaseitemsdialog.h"
#include "mergelistwidgetselectionmodel.h"
#include <QDrag>


MergeListWidget::MergeListWidget(QWidget *parent) :
    QListWidget(parent),
    m_databaseMerger(nullptr)
{
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(DropOnly);  // implement drag manually
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::NoSelection); // does not work properly
    selectionModel()->deleteLater();
    setSelectionModel(new MergeListWidgetSelectionModel(model()));
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createContextMenu(QPoint)));

    setStyleSheet(
                "QWidget:item:hover"
                "{"
                "background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #000000FF, stop: 1 #000000FF);"
                "}"
                "QWidget:item:select"
                "{"
                "background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #00000000, stop: 1 #00000000);"
                "}"
                );
}

MergeListWidget::~MergeListWidget()
{
}

QMimeData* MergeListWidget::mimeData(const QList<QListWidgetItem *> items) const
{
    Q_ASSERT(items.length() == 1);
    MergeItem* item = m_listWidgetItems.value(items.first());
    return m_databaseMerger->encodeMimeData(item);
}

bool MergeListWidget::canDrop(QListWidgetItem *item, const QMimeData *data, Qt::DropAction action) const
{
    if (item)
    {
        return canDrop(m_listWidgetItems.value(item), data, action);
    }
    else
    {
        return false;
    }
}

bool MergeListWidget::canDrop(const MergeItem *itemA, const QMimeData *data, Qt::DropAction action) const
{
    // we only support link actions
    if (action != Qt::LinkAction)
    {
        return false;
    }

    // combine only add with delete and vice versa
    MergeItem* itemB = m_databaseMerger->decodeMimeData(data);

    if (    !(itemA->type() == MergeItem::Add    && itemB->type() == MergeItem::Remove  )
         && !(itemA->type() == MergeItem::Remove && itemB->type() == MergeItem::Add     ) )
    {
        return false;
    }

    return true;
}

bool MergeListWidget::dropMimeData(int index, const QMimeData *data, Qt::DropAction action)
{
    // apparently index is sometimes off by one.
    if (index >= count() || index < 0)
    {
        // index is not guaranteed to be nice.
        return false;
    }

    QListWidgetItem* itemA = MergeListWidget::item(index);

    if (!canDrop(itemA, data, action))
    {
        // it is not guaranteed that dropMimeData is only called when canDrop returns true.
        return false;
    }

    Q_ASSERT(data->formats().length() == 1);
    MergeItem* mergeItemB = m_databaseMerger->decodeMimeData(data);
    QListWidgetItem* itemB = m_listWidgetItems.key(mergeItemB);

    join(itemA, itemB);

    return true;
}

void MergeListWidget::dragEnterEvent(QDragEnterEvent *event)
{
    QListWidget::dragEnterEvent(event);
    event->accept();
    if (canDrop(itemAt(event->pos()), event->mimeData(), event->proposedAction()))
    {
        event->acceptProposedAction();
    }
}

void MergeListWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if (canDrop(itemAt(event->pos()), event->mimeData(), event->proposedAction()))
    {
        event->acceptProposedAction();
    }
    else
    {
        event->ignore();
    }
}

void MergeListWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->ignore();
    QWidget::dragLeaveEvent(event);
}

int MergeListWidget::sizeHintForColumn(int column) const
{
    int lastColumnSize = 250;
    if (column == 0)
    {
        return viewport()->width() - lastColumnSize;
    }
    else if (column == 1)
    {
        return lastColumnSize;
    }
    else
    {
        Q_UNREACHABLE();
        return -1;
    }
}

QListWidgetItem* MergeListWidget::initListWidgetItem(MergeItem* mergeItem, QListWidgetItem* recycle)
{
    // create new item if recycling is inactive
    if (!recycle)
    {
        recycle = new QListWidgetItem(this);
    }

    // build the widget and register the item with the mergeItem
    MergeListWidgetItemWidget* mergeListWidgetItemWidget = new MergeListWidgetItemWidget(mergeItem);
    setItemWidget(recycle, mergeListWidgetItemWidget);
    connect(mergeListWidgetItemWidget, SIGNAL(clicked(MergeItem*)), this, SLOT(openCombineItemDialog(MergeItem*)));
    m_listWidgetItems.insert(recycle, mergeItem);


    // set tool tip
    QString toolTip;
    switch (mergeItem->type())
    {
    case MergeItem::Add:
        toolTip = tr("From master project");
        break;
    case MergeItem::Remove:
        toolTip = tr("From other project");
        break;
    case MergeItem::Modify:
        toolTip = tr("Combination from both projects");
        break;
    }

    recycle->setToolTip(toolTip);

    // return item
    return recycle;
}

void MergeListWidget::join(QListWidgetItem* itemA, QListWidgetItem* itemB)
{
    QModelIndex indexB = QListWidget::indexFromItem(itemB);

    MergeItem* mergeItemA = m_listWidgetItems.value(itemA);
    MergeItem* mergeItemB = m_listWidgetItems.value(itemB);
    MergeItem* joinedItem = m_databaseMerger->join(mergeItemA, mergeItemB);

    // DatabaseMerger::join has deleted mergeItem{A,B}
    m_listWidgetItems.remove(itemA);
    m_listWidgetItems.remove(itemB);
    mergeItemA = nullptr;
    mergeItemB = nullptr;

    initListWidgetItem(joinedItem, itemA);
    delete takeItem(indexB.row());
}

void MergeListWidget::split(QListWidgetItem *item)
{
    QModelIndex index = QListWidget::indexFromItem(item);
    MergeItem* mergeItem = m_listWidgetItems.value(item);
    QPair<MergeItem*, MergeItem*> masterSlaveMergeItem = m_databaseMerger->split(mergeItem);

    m_listWidgetItems.remove(item);
    mergeItem = nullptr;

    //recycle the item
    initListWidgetItem(masterSlaveMergeItem.first, item);

    // for the slave item, create a new one.
    QListWidgetItem* newListWidgetItem = initListWidgetItem(masterSlaveMergeItem.second, nullptr);
    insertItem(index.row() + 1, newListWidgetItem);
}

void MergeListWidget::createContextMenu(const QPoint &pos)
{
    QListWidgetItem* item = itemAt(pos);
    if (!item)
    {
        return;
    }

    MergeItem* mergeItem = m_listWidgetItems.value(item);
    if (mergeItem->type() == MergeItem::Modify)
    {
        QMenu* menu = new QMenu();
        connect(menu, SIGNAL(aboutToHide()), menu, SLOT(deleteLater()));

        QAction* splitAction = menu->addAction(tr("Split"));
        menu->move(mapToGlobal(pos));
        menu->show();

        connect(splitAction, &QAction::triggered, [this, item]()
        {
            this->split(item);
        });
    }
}

void MergeListWidget::setDatabaseMerger(DatabaseMerger *merger)
{
    Q_ASSERT(m_databaseMerger == nullptr);
    m_databaseMerger = merger;

    for (MergeItem* mergeItem : m_databaseMerger->mergeItems())
    {
        QListWidgetItem* listWidgetItem = initListWidgetItem(mergeItem);
        addItem(listWidgetItem);
    }

}

void MergeListWidget::openCombineItemDialog(MergeItem *mergeItem)
{
    Q_ASSERT(mergeItem->type() == MergeItem::Modify);
    CombineDatabaseItemsDialog dialog(mergeItem, this);
    dialog.exec();
}

void MergeListWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_startPos = event->pos();
    }
    QListWidget::mousePressEvent(event);
}

void MergeListWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (   event->buttons() & Qt::LeftButton
        && (m_startPos - event->pos()).manhattanLength() >= QApplication::startDragDistance())
    {
        QListWidgetItem* item = itemAt(m_startPos);
        if (item)
        {
            QDrag* drag = new QDrag(this);
            QMimeData* mimeData = MergeListWidget::mimeData({ item });
            drag->setMimeData(mimeData);
            drag->exec(Qt::LinkAction);
        }
    }
    QListWidget::mouseMoveEvent(event);
}
