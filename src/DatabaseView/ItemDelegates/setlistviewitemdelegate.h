#ifndef SETLISTVIEWITEMDELEGATE_H
#define SETLISTVIEWITEMDELEGATE_H

#include <QItemDelegate>

class SetlistItem;
class Setlist;
class SetlistViewItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit SetlistViewItemDelegate(QObject *parent = 0);

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private:
    static SetlistItem* item(const QModelIndex& index);
    static const Setlist *setlist(const QModelIndex& index);

};

#endif // SETLISTVIEWITEMDELEGATE_H
