#include "combineattachmentsdialog.h"

#include <QHBoxLayout>
#include <QDialogButtonBox>

#include "Merge/mergeitem.h"
#include "chordpatternattachmentmergewidget.h"
#include "indexedfileattachmentmergewidget.h"
#include "audioattachmentmergewidget.h"
#include "chordpatternproxyattachmentmergewidget.h"

CombineAttachmentsDialog::CombineAttachmentsDialog(MergeItem* item, QWidget *parent) :
    QDialog(parent),
    m_mergeItem(item)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    if (item->inherits({"ChordPatternAttachment"}))
    {
        m_attachmentMergeWidget = new ChordPatternAttachmentMergeWidget(item, this);
    }
    else if (item->inherits({"AudioAttachment"}))
    {
        m_attachmentMergeWidget = new AudioAttachmentMergeWidget(item, this);
    }
    else if (item->inherits({"IndexedFileAttachment"}))
    {
        m_attachmentMergeWidget = new IndexedFileAttachmentMergeWidget(item, this);
    }
    else if (item->inherits({"ChordPatternProxyAttachment"}))
    {
        m_attachmentMergeWidget = new ChordPatternProxyAttachmentMergeWidget(item, this);
    }
    else
    {
        Q_UNREACHABLE();
    }
    layout->addWidget(m_attachmentMergeWidget);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                                       Qt::Horizontal,
                                                       this);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    layout->addWidget(buttonBox);
    setLayout(layout);
}

void CombineAttachmentsDialog::accept()
{
    m_mergeItem->updateModifyDetails(m_attachmentMergeWidget->modifyDetails());
    QDialog::accept();
}
