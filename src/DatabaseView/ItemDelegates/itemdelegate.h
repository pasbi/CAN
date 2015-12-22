#ifndef ITEMDELEGATE_H
#define ITEMDELEGATE_H

#include <QItemDelegate>
#include "global.h"
#include <QAbstractProxyModel>

template<typename EditorType>
class ItemDelegate : public QItemDelegate
{
protected:
    ItemDelegate(QObject* parent = 0) :
        QItemDelegate(parent)
    {

    }

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const
    {
        QWidget* editor = new EditorType(parent);
        return editor;
    }

private:
    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        EditorType* specificEditor = qobject_assert_cast<EditorType*>(editor);
        setSpecificEditorData(specificEditor, index);
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
    {
        EditorType* specificEditor = qobject_assert_cast<EditorType*>(editor);
        setSpecificModelData(specificEditor, model, index);
    }

protected:
    virtual void setSpecificEditorData(EditorType* editor, const QModelIndex& index) const = 0;
    virtual void setSpecificModelData(EditorType* editor, QAbstractItemModel* database, const QModelIndex& index) const = 0;
};

#endif // ITEMDELEGATE_H
