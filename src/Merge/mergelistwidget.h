#ifndef MERGETREEWIDGET_H
#define MERGETREEWIDGET_H

#include <QListWidget>
#include "mergeitem.h"
#include "Database/databaseitem.h"
#include "databasemerger.h"

class Merge;

// for DatabaseItemBase
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

    void setDatabaseMerger(DatabaseMerger* m_databaseMerger);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    int sizeHintForColumn(int column) const;

private:
    bool canDrop(QListWidgetItem *item, const QMimeData* data, Qt::DropAction action) const;
    bool canDrop(const MergeItem *item, const QMimeData *data, Qt::DropAction action) const;
    void split(QListWidgetItem* oldItemWidget);
    void join(QListWidgetItem *masterItem, QListWidgetItem *slaveItem);
    QListWidgetItem* initListWidgetItem(MergeItem* mergeItem, QListWidgetItem* recycle = nullptr);


signals:
    //void combineItemDialogRequest(MergeItem*);


private slots:
    void createContextMenu(const QPoint& pos);

private:
    DatabaseMerger* m_databaseMerger;
    QMap<QListWidgetItem*, MergeItem*> m_listWidgetItems;
};


#endif // MERGETREEWIDGET_H
