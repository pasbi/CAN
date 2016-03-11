#ifndef CHORDPATTERNATTACHMENTMERGEWIDGET_H
#define CHORDPATTERNATTACHMENTMERGEWIDGET_H

#include <QWidget>
#include "attachmentmergewidgetbase.h"

class QDoubleSpinBox;
class ChordPatternEdit;

class ChordPatternAttachmentMergeWidget : public AttachmentMergeWidgetBase
{
    Q_OBJECT

public:
    explicit ChordPatternAttachmentMergeWidget(MergeItem *mergeItem, QWidget *parent = 0);
    ~ChordPatternAttachmentMergeWidget();

protected:
    void apply();

    QDoubleSpinBox* m_masterTempoSpinBox;
    QDoubleSpinBox* m_slaveTempoSpinBox;
    ChordPatternEdit* m_masterTextEdit;
    ChordPatternEdit* m_slaveTextEdit;

};

#endif // CHORDPATTERNATTACHMENTMERGEWIDGET_H
