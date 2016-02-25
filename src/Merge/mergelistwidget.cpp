#include "mergelistwidget.h"
#include "mergeitem.h"
#include <QMimeData>
#include "global.h"
#include <QDragEnterEvent>
#include <QPushButton>
#include <QMenu>
#include "mergelistwidgetitemwidget.h"

//class Song;
//class Event;
//class Attachment;

MergeListWidget::MergeListWidget(QWidget *parent) :
    QListWidget(parent)
{
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(DragDrop);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createContextMenu(QPoint)));
}

QString mimeFormat(MergeItemBase::Type type)
{
    switch (type)
    {
    case MergeItemBase::SongType:
        return "song/mergeInfo";
    case MergeItemBase::EventType:
        return "event/mergeInfo";
    case MergeItemBase::AttachmentType:
        return "attachment/mergeInfo";
    default:
        Q_UNIMPLEMENTED();
        return "";
    }
}

QMimeData* MergeListWidget::mimeData(const QList<QListWidgetItem *> items) const
{
    Q_ASSERT(items.length() == 1);
    MergeItemBase mergeItem = m_mergeItems.value(items.first());

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    stream << mergeItem;

    QMimeData* mimeData = new QMimeData();
    mimeData->setData(mimeFormat(mergeItem.type()), data);
    return mimeData;
}

bool MergeListWidget::canDrop(QListWidgetItem *item, const QMimeData *data, Qt::DropAction action) const
{
    if (item)
    {
        return canDrop(m_mergeItems.value(item), data, action);
    }
    else
    {
        return false;
    }
}

bool MergeListWidget::canDrop(const MergeItemBase &item, const QMimeData *data, Qt::DropAction action) const
{
    // we only support link actions
    if (action != Qt::LinkAction)
    {
        return false;
    }

    // combine only same-type items
    Q_ASSERT(data->formats().length() == 1);
    QString mimeFormat = data->formats().first();
    if (       (item.type() == MergeItemBase::SongType        && mimeFormat != "song/mergeInfo")
            && (item.type() == MergeItemBase::AttachmentType  && mimeFormat != "attachment/mergeInfo")
            && (item.type() == MergeItemBase::EventType       && mimeFormat != "event/mergeInfo")          )
    {
        return false;
    }

    // combine only add with delete and vice versa
    MergeItemBase itemB(data->data(mimeFormat));

    if (    !(item.origin() == MergeItemBase::MasterProject  && itemB.origin() == MergeItemBase::SlaveProject  )
         && !(item.origin() == MergeItemBase::SlaveProject   && itemB.origin() == MergeItemBase::MasterProject ) )
    {
        return false;
    }

    return true;
}

bool MergeListWidget::dropMimeData(int index, const QMimeData *data, Qt::DropAction action)
{
    if (index >= count() || index < 0)
    {
        // index is not guaranteed to be nice.
        return false;
    }

    QListWidgetItem* item = MergeListWidget::item(index);
    if(!canDrop(item, data, action))
    {
        // it is not guaranteed that dropMimeData is only called when canDrop returns true.
        return false;
    }

    Q_ASSERT(data->formats().length() == 1);
    QString mimeFormat = data->formats().first();
    MergeItemBase source(data->data(mimeFormat));

    join(item, m_mergeItems.key(source));

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

void MergeListWidget::join(QListWidgetItem* targetWidgetItem, QListWidgetItem *sourceWidgetItem)
{
    MergeItemBase& targetMergeItem = m_mergeItems[targetWidgetItem];
    const MergeItemBase& sourceMergeItem = m_mergeItems[sourceWidgetItem];

    // find master and slave pointers and labels
    void* masterPointer = nullptr;
    void* slavePointer = nullptr;
    QString masterLabel, slaveLabel;
    if (sourceMergeItem.origin() == MergeItemBase::MasterProject)
    {
        masterPointer = sourceMergeItem.pointer();
        slavePointer = targetMergeItem.pointer();
        masterLabel = sourceMergeItem.baseLabel();
        slaveLabel = targetMergeItem.baseLabel();
    }
    else if (sourceMergeItem.origin() == MergeItemBase::SlaveProject)
    {
        masterPointer = targetMergeItem.pointer();
        slavePointer = sourceMergeItem.pointer();
        masterLabel = targetMergeItem.baseLabel();
        slaveLabel = sourceMergeItem.baseLabel();
    }
    else
    {
        // only master/slave items can be joined. Both/No items cannot be joined.
        Q_UNREACHABLE();
    }

    // update target fields
    targetMergeItem.setMasterSlavePointer(masterPointer, slavePointer);
    targetMergeItem.setMasterSlaveLabel(masterLabel, slaveLabel);
    targetMergeItem.setOrigin(MergeItemBase::BothProjects);

    // update target item widget
    setItemWidget(targetWidgetItem, new MergeListWidgetItemWidget(targetMergeItem));

    // remove source item widget
    takeItem(row(sourceWidgetItem));
    m_mergeItems.remove(sourceWidgetItem);
    delete sourceWidgetItem;
    sourceWidgetItem = nullptr;
}

void MergeListWidget::split(QListWidgetItem *targetItemWidget)
{
    // add a new item
    QListWidgetItem *sourceItemWidget = new QListWidgetItem();
    insertItem(row(targetItemWidget) + 1, sourceItemWidget);

    // update the merge items
    MergeItemBase copy = m_mergeItems[targetItemWidget];
    MergeItemBase& targetMergeItem = m_mergeItems[targetItemWidget];
    MergeItemBase sourceMergeItem = copy;    // make a copy

    targetMergeItem.setOrigin( MergeItemBase::MasterProject );
    sourceMergeItem.setOrigin( MergeItemBase::SlaveProject );
    targetMergeItem.setAction( MergeItemBase::AddItemAction );
    sourceMergeItem.setAction( MergeItemBase::AddItemAction );
    targetMergeItem.setPointer( copy.masterPointer() );
    sourceMergeItem.setPointer( copy.slavePointer() );
    targetMergeItem.setLabel( copy.masterBaseLabel() );
    sourceMergeItem.setLabel( copy.slaveBaseLabel() );

    m_mergeItems.insert(sourceItemWidget, sourceMergeItem);


    // update the widgets
    setItemWidget(targetItemWidget, new MergeListWidgetItemWidget(m_mergeItems[targetItemWidget]) );
    setItemWidget(sourceItemWidget, new MergeListWidgetItemWidget(m_mergeItems[sourceItemWidget]) );
}

void MergeListWidget::createContextMenu(const QPoint &pos)
{
    QListWidgetItem* item = itemAt(pos);
    if (!item)
    {
        return;
    }
    MergeItemBase mib = m_mergeItems.value(item);
    if (mib.origin() == MergeItemBase::BothProjects)
    {
        QMenu* menu = new QMenu();
        connect(menu, SIGNAL(aboutToHide()), menu, SLOT(deleteLater()));

        QAction* splitAction = menu->addAction(tr("Split"));
        menu->move(mapToGlobal(pos));
        menu->show();

        connect(splitAction, &QAction::triggered, [this, item]()
        {
            split(item);
        });
    }
}

void MergeListWidget::setItems(const QList<MergeItemBase> &items)
{
    Q_ASSERT(m_mergeItems.isEmpty());

    for (const MergeItemBase& item : items)
    {
        QListWidgetItem* listWidgetItem = new QListWidgetItem();
        addItem(listWidgetItem);
        m_mergeItems.insert(listWidgetItem, item);
        setItemWidget(listWidgetItem, new MergeListWidgetItemWidget(m_mergeItems[listWidgetItem]));
    }
}

