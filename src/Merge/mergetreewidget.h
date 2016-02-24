#ifndef MERGETREEWIDGET_H
#define MERGETREEWIDGET_H

#include <QTreeWidget>

class MergeTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit MergeTreeWidget(QWidget *parent = 0);

    bool dropMimeData(QTreeWidgetItem *item, int index, const QMimeData *data, Qt::DropAction action);
    QMimeData* mimeData(const QList<QTreeWidgetItem *> items) const;
    Qt::DropActions supportedDropActions() const { return Qt::LinkAction; }
    Qt::DropAction defaultDropAction() const { return Qt::LinkAction; }
    QTreeWidgetItem* root() const;

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);

private:
    bool canDrop(const QPoint &pos, const QMimeData* data, Qt::DropAction action) const;
    bool canDrop(const QTreeWidgetItem* item, const QMimeData *data, Qt::DropAction action) const;
};


#endif // MERGETREEWIDGET_H
