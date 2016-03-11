#ifndef COMBINEATTACHMENTSDIALOG_H
#define COMBINEATTACHMENTSDIALOG_H

#include <QDialog>

class MergeItem;

class CombineAttachmentsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CombineAttachmentsDialog(MergeItem *item, QWidget *parent = 0);

signals:

public slots:

};

#endif // COMBINEATTACHMENTSDIALOG_H
