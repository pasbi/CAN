#ifndef MERGEDIALOG_H
#define MERGEDIALOG_H

#include <QDialog>
#include "merge.h"

namespace Ui {
class MergeDialog;
}

class EventMergeWidget;
class SongMergeWidget;
class MergeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MergeDialog(Merge* merge, QWidget *parent = 0);
    ~MergeDialog();

    QMap<Song*, MergeItemBase*> attachmentMergeItems() const { Q_UNIMPLEMENTED(); return QMap<Song*, MergeItemBase*>(); }

 private:
    Ui::MergeDialog *ui;
    Merge* m_merger;
    void setMerger(Merge* merger);
};

#endif // MERGEDIALOG_H
