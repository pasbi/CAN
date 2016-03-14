#include "chordpatternproxyattachmentmergewidget.h"
#include "Attachments/ChordPatternAttachment/chordpatternproxyattachment.h"
#include "Attachments/ChordPatternAttachment/chordpatternattachment.h"
#include "Database/SongDatabase/song.h"
#include <QLineEdit>
#include <QSpinBox>

QString label(Attachment* attachment)
{
    if (attachment)
    {
        return attachment->name();
    }
    else
    {
        return ChordPatternProxyAttachmentMergeWidget::tr("None");
    }
}

ChordPatternProxyAttachmentMergeWidget::ChordPatternProxyAttachmentMergeWidget(MergeItem *item, QWidget *parent) :
    AttachmentMergeWidget(item, parent)
{
    ChordPatternProxyAttachment* cppaMaster = static_cast<ChordPatternProxyAttachment*>(item->master());
    ChordPatternProxyAttachment* cppaSlave = static_cast<ChordPatternProxyAttachment*>(item->slave());

    ChordPatternAttachment* sourceMaster = cppaMaster->source();
    ChordPatternAttachment* sourceSlave = cppaSlave->source();

    int masterID = sourceMaster ? sourceMaster->song()->attachments().indexOf(sourceMaster) : -1;
    int slaveID = sourceSlave ? sourceSlave->song()->attachments().indexOf(sourceSlave) : -1;

    if (masterID != slaveID)
    {
        m_masterEdit = new QLineEdit(this);
        m_slaveEdit = new QLineEdit(this);

        m_masterEdit->setReadOnly(true);
        m_slaveEdit->setReadOnly(true);

        m_masterEdit->setText(label(cppaMaster->source()));
        m_slaveEdit->setText(label(cppaSlave->source()));

        addEditorWidget(m_masterEdit, m_slaveEdit, "sourceChordPatternAttachment" );
    }
    else
    {
        m_masterEdit = nullptr;
        m_slaveEdit = nullptr;
    }

    if (cppaMaster->transpose() != cppaSlave->transpose())
    {
        m_masterTranspose = new QSpinBox(this);
        m_slaveTranspose = new QSpinBox(this);

        m_masterTranspose->setReadOnly(true);
        m_slaveTranspose->setReadOnly(true);

        m_masterTranspose->setRange(-6, 5);
        m_slaveTranspose->setRange(-6, 5);

        m_masterTranspose->setValue((cppaMaster->transpose() + 6) % 12 - 6);
        m_slaveTranspose->setValue((cppaSlave->transpose() + 6) % 12 - 6);

        addEditorWidget(m_masterTranspose, m_slaveTranspose, "transpose", tr("Transpose"));
    }
    else
    {
        m_masterTranspose = nullptr;
        m_slaveTranspose = nullptr;
    }
}
