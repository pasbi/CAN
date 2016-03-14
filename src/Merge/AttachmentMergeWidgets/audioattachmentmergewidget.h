#ifndef AUDIOATTACHMENTMERGEWIDGET_H
#define AUDIOATTACHMENTMERGEWIDGET_H

#include "indexedfileattachmentmergewidget.h"

class QTableWidget;
class AudioAttachmentMergeWidget : public IndexedFileAttachmentMergeWidget
{
public:
    AudioAttachmentMergeWidget(MergeItem* mergeItem, QWidget* parent = nullptr);

    QTableWidget* m_masterSections;
    QTableWidget* m_slaveSections;
};

#endif // AUDIOATTACHMENTMERGEWIDGET_H
