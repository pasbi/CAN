#ifndef KEYDELEGATE_H
#define KEYDELEGATE_H

#include "itemdelegate.h"
#include "keyeditor.h"

class KeyDelegate : public ItemDelegate<KeyEditor>
{
public:
    KeyDelegate(QObject* parent = nullptr);

private:
    void setSpecificEditorData(KeyEditor* editor, const QModelIndex& index) const;
    void setSpecificModelData(KeyEditor *editor, QAbstractItemModel *database, const QModelIndex &index) const;
};

#endif // KEYDELEGATE_H
