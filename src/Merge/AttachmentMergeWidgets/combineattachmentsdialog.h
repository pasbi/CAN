#ifndef COMBINEATTACHMENTSDIALOG_H
#define COMBINEATTACHMENTSDIALOG_H

#include <QDialog>

class MergeItem;
class AttachmentMergeWidgetBase;
class CombineAttachmentsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CombineAttachmentsDialog(MergeItem *item, QWidget *parent = 0);

    void accept();

private:
    AttachmentMergeWidgetBase* m_attachmentMergeWidget;
    MergeItem* m_mergeItem;

};

#endif // COMBINEATTACHMENTSDIALOG_H
