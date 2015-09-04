#ifndef ITEMDELEGATE_H
#define ITEMDELEGATE_H

#include <QItemDelegate>
#include "global.h"

template<typename T> class Database;
template<typename T, typename EditorType>
class ItemDelegate : public QItemDelegate
{
protected:
    ItemDelegate(QObject* parent = 0) :
        QItemDelegate(parent)
    {

    }

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        Q_UNUSED(index);
        Q_UNUSED(option);
        QWidget* editor = new EditorType(parent);
        return editor;
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        EditorType* specificEditor = qobject_assert_cast<EditorType*>(editor);
        setEditorData(specificEditor, index);
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
    {
        EditorType* specificEditor = qobject_assert_cast<EditorType*>(editor);
        Database<T>* database = static_cast<Database<T>*>(model);
        setModelData(specificEditor, database, index);
    }

    virtual void setEditorData(EditorType* editor, const QModelIndex& index) const = 0;
    virtual void setModelData(EditorType* editor, Database<T>* database, const QModelIndex& index) const = 0;
};

#endif // ITEMDELEGATE_H
