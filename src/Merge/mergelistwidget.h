#ifndef MERGETREEWIDGET_H
#define MERGETREEWIDGET_H

#include <QListWidget>
#include "mergeitem.h"

class Merge;

class MergeListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit MergeListWidget(QWidget *parent = 0);
    ~MergeListWidget();

    bool dropMimeData(int index, const QMimeData *data, Qt::DropAction action);
    QMimeData* mimeData(const QList<QListWidgetItem *> items) const;
    Qt::DropActions supportedDropActions() const { return Qt::LinkAction; }
    Qt::DropAction defaultDropAction() const { return Qt::LinkAction; }
    void setMergeItems(const QList<MergeItemBase *> &items);
    QList<MergeItemBase *> mergeItems() const;
    Merge* merge() const { return m_merge; }
    void setMerge(Merge* merge);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    int sizeHintForColumn(int column) const;

private:
    bool canDrop(QListWidgetItem *item, const QMimeData* data, Qt::DropAction action) const;
    bool canDrop(const MergeItemBase *item, const QMimeData *data, Qt::DropAction action) const;
    void split(QListWidgetItem* oldItemWidget);
    void join(QListWidgetItem* targetWidgetItem, QListWidgetItem* sourceWidgetItem);

signals:
    void combineItemDialogRequest(MergeItemBase*);


private slots:
    void createContextMenu(const QPoint& pos);
private:
    QMap<QListWidgetItem*, MergeItemBase*> m_mergeItems;
    Merge* m_merge;
};


#endif // MERGETREEWIDGET_H
