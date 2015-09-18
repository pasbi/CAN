#ifndef EDITABLECOMBOBOXDELEGATE_H
#define EDITABLECOMBOBOXDELEGATE_H

#include "itemdelegate.h"


class QComboBox;
class EditableComboBoxDelegate : public ItemDelegate<QComboBox>
{
public:
    EditableComboBoxDelegate(QObject* parent = 0);

private:
    void setModelData(QComboBox *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setEditorData(QComboBox *editor, const QModelIndex &index) const;
};

#endif // EDITABLECOMBOBOXDELEGATE_H
