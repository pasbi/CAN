#ifndef CHORDPATTERNPROXYATTACHMENTMERGEWIDGET_H
#define CHORDPATTERNPROXYATTACHMENTMERGEWIDGET_H

#include "attachmentmergewidget.h"

class QSpinBox;
class ChordPatternProxyAttachmentMergeWidget : public AttachmentMergeWidget
{
public:
    ChordPatternProxyAttachmentMergeWidget(MergeItem* item, QWidget* parent = nullptr);

private:
    QLineEdit* m_masterEdit;
    QLineEdit* m_slaveEdit;
    QSpinBox* m_masterTranspose;
    QSpinBox* m_slaveTranspose;
};

#endif // CHORDPATTERNPROXYATTACHMENTMERGEWIDGET_H
