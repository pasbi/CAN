#include "mergelistwidget.h"
#include "mergeitem.h"
#include <QMimeData>
#include "global.h"
#include <QDragEnterEvent>
#include <QPushButton>
#include <QMenu>
#include "mergelistwidgetitemwidget.h"
#include "merge.h"
#include "combinesongsdialog.h"
#include "mergelistwidgetselectionmodel.h"


MergeListWidget::MergeListWidget(QWidget *parent) :
    QListWidget(parent),
    m_merge(nullptr)
{
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(DragDrop);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection); // does not work properly
    selectionModel()->deleteLater();
    setSelectionModel(new MergeListWidgetSelectionModel(model()));
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createContextMenu(QPoint)));
}

MergeListWidget::~MergeListWidget()
{
}

QMimeData* MergeListWidget::mimeData(const QList<QListWidgetItem *> items) const
{
    Q_ASSERT(items.length() == 1);
    return merge()->encodeMimeData( m_mergeItems.value(items.first()) );
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

bool MergeListWidget::canDrop(const MergeItemBase* item, const QMimeData *data, Qt::DropAction action) const
{
    // we only support link actions
    if (action != Qt::LinkAction)
    {
        return false;
    }

    // combine only same-type items
    Q_ASSERT(data->formats().length() == 1);
    QString mimeFormat = data->formats().first();
    if (       (item->type() == MergeItemBase::SongType        && mimeFormat != "merge")
            && (item->type() == MergeItemBase::AttachmentType  && mimeFormat != "merge")
            && (item->type() == MergeItemBase::EventType       && mimeFormat != "merge")          )
    {
        return false;
    }

    // combine only add with delete and vice versa
    MergeItemBase* itemB = merge()->decodeMimeData(data);

    if (    !(item->origin() == MergeItemBase::MasterProject  && itemB->origin() == MergeItemBase::SlaveProject  )
         && !(item->origin() == MergeItemBase::SlaveProject   && itemB->origin() == MergeItemBase::MasterProject ) )
    {
        return false;
    }

    if (item->type() != itemB->type())
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

    QListWidgetItem* item = MergeListWidget::item(index);
    if(!canDrop(item, data, action))
    {
        // it is not guaranteed that dropMimeData is only called when canDrop returns true.
        return false;
    }

    Q_ASSERT(data->formats().length() == 1);
    MergeItemBase* source = merge()->decodeMimeData(data);

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
    MergeItemBase* targetMergeItem = m_mergeItems[targetWidgetItem];
    const MergeItemBase* sourceMergeItem = m_mergeItems[sourceWidgetItem];

    Q_ASSERT(targetMergeItem->type() == sourceMergeItem->type());

    // find master and slave pointers and labels
    void* masterPointer = nullptr;
    void* slavePointer = nullptr;
    QString masterLabel, slaveLabel;
    if (sourceMergeItem->origin() == MergeItemBase::MasterProject)
    {
        masterPointer = sourceMergeItem->pointer();
        slavePointer = targetMergeItem->pointer();
        masterLabel = sourceMergeItem->baseLabel();
        slaveLabel = targetMergeItem->baseLabel();
    }
    else if (sourceMergeItem->origin() == MergeItemBase::SlaveProject)
    {
        masterPointer = targetMergeItem->pointer();
        slavePointer = sourceMergeItem->pointer();
        masterLabel = targetMergeItem->baseLabel();
        slaveLabel = sourceMergeItem->baseLabel();
    }
    else
    {
        // only master/slave items can be joined. Both/No items cannot be joined.
        Q_UNREACHABLE();
    }

    // update target fields
    targetMergeItem->setMasterSlavePointer(masterPointer, slavePointer);
    targetMergeItem->setMasterSlaveLabel(masterLabel, slaveLabel);
    targetMergeItem->setOrigin(MergeItemBase::BothProjects);
    targetMergeItem->setAction(MergeItemBase::ModifyItemAction);
    targetMergeItem->initializeCombinationObject();  // copy master object to combination object

    // update target item widget
    MergeListWidgetItemWidget* itemWidget = new MergeListWidgetItemWidget(targetMergeItem);
    connect(itemWidget, &MergeListWidgetItemWidget::clicked, [this, targetMergeItem]()
    {
        switch (targetMergeItem->type())
        {
        case MergeItemBase::SongType:
        {
            CombineSongsDialog dialog(targetMergeItem->masterPointer<Song>(),
                                      targetMergeItem->slavePointer<Song>(), this);

            if (dialog.exec() == QDialog::Accepted)
            {
                targetMergeItem->deleteCombinationObject();
                targetMergeItem->setCombinationPointer(dialog.combination());
            }
            break;
        }
        case MergeItemBase::EventType:
            Q_UNIMPLEMENTED();
            break;
        case MergeItemBase::AttachmentType:
            Q_UNIMPLEMENTED();
            break;
        default:
            Q_UNREACHABLE();
            break;
        }
    });
    setItemWidget(targetWidgetItem, itemWidget);

    // remove source item widget
    takeItem(row(sourceWidgetItem));
    m_mergeItems.remove(sourceWidgetItem);
    merge()->removeMergeItem(sourceMergeItem);
    delete sourceMergeItem;
    sourceMergeItem = nullptr;
    delete sourceWidgetItem;
    sourceWidgetItem = nullptr;
}

void MergeListWidget::split(QListWidgetItem *oldItemWidget)
{
    // add a new item
    QListWidgetItem *newItemWidget = new QListWidgetItem();
    insertItem(row(oldItemWidget) + 1, newItemWidget);

    // update the merge items
    MergeItemBase* oldMergeItem = m_mergeItems[oldItemWidget];
    MergeItemBase* newMergeItem = new MergeItemBase( MergeItemBase::SlaveProject,
                                                     oldMergeItem->type(),
                                                     oldMergeItem->slavePointer(),
                                                     oldMergeItem->slaveBaseLabel() );

    newMergeItem->setAction( MergeItemBase::AddItemAction );

    oldMergeItem->setOrigin( MergeItemBase::MasterProject );
    oldMergeItem->setAction( MergeItemBase::AddItemAction );
    oldMergeItem->setPointer( oldMergeItem->masterPointer() );
    oldMergeItem->setLabel( oldMergeItem->masterBaseLabel() );

    // we don't need the combination object anymore
    oldMergeItem->deleteCombinationObject();

    m_mergeItems.insert(newItemWidget, newMergeItem);
    merge()->addMergeItem(newMergeItem);


    // update the widgets
    setItemWidget(oldItemWidget, new MergeListWidgetItemWidget(m_mergeItems[oldItemWidget]) );
    setItemWidget(newItemWidget, new MergeListWidgetItemWidget(m_mergeItems[newItemWidget]) );
}

void MergeListWidget::createContextMenu(const QPoint &pos)
{
    QListWidgetItem* item = itemAt(pos);
    if (!item)
    {
        return;
    }
    MergeItemBase* mib = m_mergeItems.value(item);
    if (mib->origin() == MergeItemBase::BothProjects)
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

void MergeListWidget::setMergeItems(const QList<MergeItemBase*> &items)
{
    for (MergeItemBase* item : items)
    {
        QListWidgetItem* listWidgetItem = new QListWidgetItem();
        addItem(listWidgetItem);
        m_mergeItems.insert(listWidgetItem, item);
        setItemWidget(listWidgetItem, new MergeListWidgetItemWidget(m_mergeItems[listWidgetItem]));
    }
}

QList<MergeItemBase*> MergeListWidget::mergeItems() const
{
    return m_mergeItems.values();
}

void MergeListWidget::setMerge(Merge* merge, QList<MergeItemBase*> mergeItems)
{
    Q_ASSERT(!m_merge);
    m_merge = merge;
    setMergeItems(mergeItems);
}

