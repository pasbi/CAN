#include "indexedfileattachmentmergewidget.h"
#include "Attachments/indexedfileattachment.h"
#include <QLineEdit>

IndexedFileAttachmentMergeWidget::IndexedFileAttachmentMergeWidget(MergeItem* mergeItem, QWidget* parent) :
    AttachmentMergeWidget(mergeItem, parent)
{

    IndexedFileAttachment* ifaMaster = static_cast<IndexedFileAttachment*>(mergeItem->master());
    IndexedFileAttachment* ifaSlave = static_cast<IndexedFileAttachment*>(mergeItem->slave());

    if (ifaMaster->hash() != ifaSlave->hash())
    {
        m_masterEdit = new QLineEdit(this);
        m_slaveEdit = new QLineEdit(this);

        m_masterEdit->setReadOnly(true);
        m_slaveEdit->setReadOnly(true);

        m_masterEdit->setText(ifaMaster->hash().toHex());
        m_slaveEdit->setText(ifaSlave->hash().toHex());

        addEditorWidget(m_masterEdit, m_slaveEdit, "hash");
    }
    else
    {
        m_masterEdit = nullptr;
        m_slaveEdit = nullptr;
    }
}
