#ifndef DURATIONDELEGATE_H
#define DURATIONDELEGATE_H

#include "itemdelegate.h"
#include "durationeditor.h"


class DurationDelegate : public ItemDelegate<DurationEditor>
{
public:
    DurationDelegate(QObject* parent = 0);

private:
    void setModelData(DurationEditor *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setEditorData(DurationEditor *editor, const QModelIndex &index) const;

};

#endif // DURATIONDELEGATE_H
