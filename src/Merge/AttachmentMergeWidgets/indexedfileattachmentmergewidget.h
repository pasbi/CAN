#ifndef INDEXEDFILEATTACHMENTMERGEWIDGET_H
#define INDEXEDFILEATTACHMENTMERGEWIDGET_H

#include "attachmentmergewidget.h"

class QLineEdit;

class IndexedFileAttachmentMergeWidget : public AttachmentMergeWidget
{
public:
    IndexedFileAttachmentMergeWidget(MergeItem *mergeItem, QWidget *parent = nullptr);

    QLineEdit* m_masterEdit;
    QLineEdit* m_slaveEdit;


};

#endif // INDEXEDFILEATTACHMENTMERGEWIDGET_H
