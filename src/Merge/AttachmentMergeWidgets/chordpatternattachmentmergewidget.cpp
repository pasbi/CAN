#include "chordpatternattachmentmergewidget.h"

#include <QDoubleSpinBox>
#include "AttachmentView/chordpatternedit.h"

#include "Attachments/ChordPatternAttachment/chordpatternattachment.h"

ChordPatternAttachmentMergeWidget::ChordPatternAttachmentMergeWidget(MergeItem* mergeItem, QWidget *parent) :
    AttachmentMergeWidgetBase(mergeItem, parent)
{
    ChordPatternAttachment* cpaMaster = static_cast<ChordPatternAttachment*>(mergeItem->master());
    ChordPatternAttachment* cpaSlave = static_cast<ChordPatternAttachment*>(mergeItem->slave());

    if (cpaMaster->chordPattern() != cpaSlave->chordPattern())
    {
        m_masterTextEdit = new ChordPatternEdit(this);
        m_slaveTextEdit = new ChordPatternEdit(this);
        connect(m_masterTextEdit, SIGNAL(textChanged()), m_masterTextEdit, SLOT(update()));
        addEditorWidget(m_masterTextEdit, m_slaveTextEdit, "chordPattern");
        m_masterTextEdit->setChordPattern(cpaMaster->chordPattern());
        m_slaveTextEdit->setChordPattern(cpaSlave->chordPattern());
    }

    if (cpaMaster->scrollDownTempo() != cpaSlave->scrollDownTempo())
    {
        m_masterTempoSpinBox = new QDoubleSpinBox(this);
        m_slaveTempoSpinBox = new QDoubleSpinBox(this);
        addEditorWidget(m_masterTempoSpinBox, m_slaveTempoSpinBox, "scrollDownTempo", tr("Tempo"));
        m_masterTempoSpinBox->setValue(cpaMaster->scrollDownTempo());
        m_slaveTempoSpinBox->setValue(cpaSlave->scrollDownTempo());
    }
}

ChordPatternAttachmentMergeWidget::~ChordPatternAttachmentMergeWidget()
{
}
