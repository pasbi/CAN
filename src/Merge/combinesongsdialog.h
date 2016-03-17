#ifndef COMBINESONGSDIALOH_H
#define COMBINESONGSDIALOH_H

#include "combinedatabaseitemsdialog.h"
#include "QItemSelection"
#include "mergeitem.h"

class MergeListWidget;
class CombineSongsDialog : public CombineDatabaseItemsDialog
{
    Q_OBJECT

public:
    explicit CombineSongsDialog(DatabaseMergerBase *mergeBase, MergeItem* mergeItem, QWidget* parent = nullptr);
    ~CombineSongsDialog();

private:
    MergeListWidget* m_mergeListWidget;
};

#endif // COMBINESONGSDIALOH_H
