#include "attachmentmergewidget.h"
#include <QLineEdit>
#include "Attachments/attachment.h"

AttachmentMergeWidget::AttachmentMergeWidget(MergeItem* mergeItem, QWidget *parent) :
    AttachmentMergeWidgetBase(mergeItem, parent)
{

    Attachment* masterAttachment = static_cast<Attachment*>(mergeItem->master());
    Attachment* slaveAttachment = static_cast<Attachment*>(mergeItem->slave());

    if (masterAttachment->name() != slaveAttachment->name())
    {
        m_masterEdit = new QLineEdit(this);
        m_slaveEdit = new QLineEdit(this);

        m_masterEdit->setReadOnly(true);
        m_slaveEdit->setReadOnly(true);

        m_masterEdit->setText(masterAttachment->name());
        m_slaveEdit->setText(slaveAttachment->name());

        addEditorWidget(m_masterEdit, m_slaveEdit, "name");
    }
    else
    {
        m_masterEdit = nullptr;
        m_slaveEdit = nullptr;
    }
}

AttachmentMergeWidget::~AttachmentMergeWidget()
{
}
