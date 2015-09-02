#ifndef TYPECOMBOBOXDELEGATE_H
#define TYPECOMBOBOXDELEGATE_H

#include <QItemDelegate>

class TypeComboBoxDelegate : public QItemDelegate
{
public:
    TypeComboBoxDelegate( QObject* parent = 0 );
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


#endif // TYPECOMBOBOXDELEGATE_H
