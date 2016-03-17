#ifndef COMBINEEVENTSDIALOG_H
#define COMBINEEVENTSDIALOG_H

#include "combinedatabaseitemsdialog.h"

namespace Ui
{
    class CombineEventsDialog;
}

class CombineEventsDialog : public CombineDatabaseItemsDialog
{
public:
    CombineEventsDialog(DatabaseMergerBase* databaseMerger, MergeItem* mergItem, QWidget* parent = nullptr);
    ~CombineEventsDialog();

private:
    Ui::CombineEventsDialog* ui;

};

#endif // COMBINEEVENTSDIALOG_H
