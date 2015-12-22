#ifndef DURATIONDELEGATE_H
#define DURATIONDELEGATE_H

#include "itemdelegate.h"
#include "durationeditor.h"


class DurationDelegate : public ItemDelegate<DurationEditor>
{
public:
    DurationDelegate(QObject* parent = 0);

private:
    void setSpecificModelData(DurationEditor *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setSpecificEditorData(DurationEditor *editor, const QModelIndex &index) const;

};

#endif // DURATIONDELEGATE_H
