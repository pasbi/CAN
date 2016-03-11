#ifndef COMBINEATTACHMENTSDIALOG_H
#define COMBINEATTACHMENTSDIALOG_H

#include <QDialog>

namespace Ui {
class CombineAttachmentsDialog;
}

class MergeItem;
class CombineAttachmentsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CombineAttachmentsDialog(MergeItem* mergeItem, QWidget *parent = 0);
    ~CombineAttachmentsDialog();

private:
    Ui::CombineAttachmentsDialog *ui;
};

#endif // COMBINEATTACHMENTSDIALOG_H
