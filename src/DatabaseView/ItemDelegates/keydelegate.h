#ifndef KEYDELEGATE_H
#define KEYDELEGATE_H

#include "itemdelegate.h"

class KeyEditor : public QWidget
{
    Q_OBJECT
public:
    KeyEditor(QWidget* parent = nullptr) :
        QWidget(parent)
    {

    }
};

class KeyDelegate : public ItemDelegate<KeyEditor>
{
public:
    KeyDelegate(QObject* parent = nullptr);

    void setEditorData(KeyEditor* editor, const QModelIndex& index) const;
    void setModelData(KeyEditor *editor, QAbstractItemModel *database, const QModelIndex &index) const;
};

#endif // KEYDELEGATE_H
