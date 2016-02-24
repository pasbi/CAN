#include "mergetreewidget.h"
#include "mergetreeitem.h"
#include <QMimeData>
#include "global.h"
#include <QDragEnterEvent>
#include <QPushButton>

MergeTreeWidget::MergeTreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(DragDrop);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
}

QMimeData* MergeTreeWidget::mimeData(const QList<QTreeWidgetItem *> items) const
{
    Q_ASSERT(items.length() == 1);

    QString mimeFormat;
    MergeInfoBase mergeInfoBase = *static_cast<MergeTreeItemBase*>(items.first())->mergeInfoBase();
    switch (static_cast<MergeTreeItemBase*>(items.first())->type())
    {
    case MergeTreeItemBase::SongType:
        mimeFormat = "song/mergeInfo";
        break;
    case MergeTreeItemBase::AttachmentType:
        mimeFormat = "attachment/mergeInfo";
        break;
    case MergeTreeItemBase::EventType:
        mimeFormat = "event/mergeInfo";
        break;
    }

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    stream << mergeInfoBase;

    QMimeData* mimeData = new QMimeData();
    mimeData->setData(mimeFormat, data);
    return mimeData;
}

bool MergeTreeWidget::canDrop(const QPoint &pos, const QMimeData *data, Qt::DropAction action) const
{
    return canDrop(itemAt(pos), data, action);
}

bool MergeTreeWidget::canDrop(const QTreeWidgetItem* item, const QMimeData *data, Qt::DropAction action) const
{
    // we only support link actions
    if (action != Qt::LinkAction)
    {
        qDebug() << "canDrop: wrong action";
        return false;
    }

    // item must exist and it must not be root.
    if (!item || !item->parent())
    {
        qDebug() << "canDrop: missing item";
        return false;
    }

    // combine only same-type items
    Q_ASSERT(data->formats().length() == 1);
    QString mimeFormat = data->formats().first();
    const MergeTreeItemBase* mItem = static_cast<const MergeTreeItemBase*>(item);
    if (       (mItem->type() == MergeTreeItemBase::SongType        && mimeFormat != "song/mergeInfo")
            && (mItem->type() == MergeTreeItemBase::AttachmentType  && mimeFormat != "attachment/mergeInfo")
            && (mItem->type() == MergeTreeItemBase::EventType       && mimeFormat != "event/mergeInfo")          )
    {
        qDebug() << "canDrop: incompatible a";
        return false;
    }

    // combine only add with delete and vice versa
    MergeInfoBase mibA = *mItem->mergeInfoBase();
    QDataStream stream(data->data(mimeFormat));
    MergeInfoBase mibB(MergeInfoBase::Other, nullptr);
    stream >> mibB;


    if (    !(mibA.type() == MergeInfoBase::AddItemType    && mibB.type() == MergeInfoBase::DeleteItemType)
         && !(mibA.type() == MergeInfoBase::DeleteItemType && mibB.type() == MergeInfoBase::AddItemType   ) )
    {
        qDebug() << "canDrop: incompatible b";
        return false;
    }

    return true;
}

QTreeWidgetItem* findTreeWidgetItem(QTreeWidgetItem* root, const void* dataPointer)
{
    // if root is the desired item, return it
    MergeTreeItemBase* mtib = dynamic_cast<MergeTreeItemBase*>(root);
    if (mtib && mtib->mergeInfoBase()->dataPointer() == dataPointer)
    {
        return root;
    }

    // continue recursively
    for (int i = 0; i < root->childCount(); ++i)
    {
        QTreeWidgetItem* item = findTreeWidgetItem(root->child(i), dataPointer);
        if (item)
        {
            return item;
        }
    }

    // if nothing was found, return nullptr
    return nullptr;
}

bool MergeTreeWidget::dropMimeData(QTreeWidgetItem *item, int /*index*/, const QMimeData *data, Qt::DropAction action)
{
    if (!canDrop(item, data, action))
    {
        qDebug() << "cannot drop.";
        return false;
    }

    Q_ASSERT(data->formats().length() == 1);
    QString mimeFormat = data->formats().first();
    QDataStream stream(data->data(mimeFormat));

    MergeInfoBase miB(MergeInfoBase::Other, nullptr);
    stream >> miB;

    MergeTreeItemBase* source = static_cast<MergeTreeItemBase*>(findTreeWidgetItem(root(), miB.dataPointer()));
    MergeTreeItemBase* target = static_cast<MergeTreeItemBase*>(item);
    Q_ASSERT(source);
    Q_ASSERT(target);
    Q_ASSERT(target->parent() == source->parent());


    void* masterPointer = nullptr;
    void* slavePointer = nullptr;
    if (source->mergeInfoBase()->type() == MergeInfoBase::DeleteItemType)
    {
        masterPointer = source->mergeInfoBase()->dataPointer();
        slavePointer = target->mergeInfoBase()->dataPointer();
    }
    else
    {
        masterPointer = target->mergeInfoBase()->dataPointer();
        slavePointer = source->mergeInfoBase()->dataPointer();
    }

    target->mergeInfoBase()->setMasterSlavePointer(masterPointer, slavePointer);
    itemChanged(target, 0);
    removeItemWidget(target, 1);
    setItemWidget(target, 1, new QPushButton());
    target->updateLabel();

    source->parent()->removeChild(source);
    delete source;  //TODO delete only once!
    source = nullptr;

    return true;
}

void MergeTreeWidget::dragEnterEvent(QDragEnterEvent *event)
{
    QTreeWidget::dragEnterEvent(event);
    event->accept();
    if (canDrop(event->pos(), event->mimeData(), event->proposedAction()))
    {
        event->acceptProposedAction();
    }
}

void MergeTreeWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if (canDrop(event->pos(), event->mimeData(), event->proposedAction()))
    {
        event->acceptProposedAction();
    }
    else
    {
        event->ignore();
    }
}

void MergeTreeWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->ignore();
    QWidget::dragLeaveEvent(event);
}

QTreeWidgetItem* MergeTreeWidget::root() const
{
    if (topLevelItemCount() == 0)
    {
        return nullptr;
    }
    else
    {
        Q_ASSERT(topLevelItemCount() == 1);
        return topLevelItem(0);
    }
}
