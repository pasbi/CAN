#include "combineattachmentsdialog.h"

#include <QHBoxLayout>

#include "Merge/mergeitem.h"
#include "chordpatternattachmentmergewidget.h"
#include <QDialogButtonBox>

CombineAttachmentsDialog::CombineAttachmentsDialog(MergeItem* item, QWidget *parent) :
    QDialog(parent),
    m_mergeItem(item)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    if (item->inherits({"ChordPatternAttachment"}))
    {
        m_attachmentMergeWidget = new ChordPatternAttachmentMergeWidget(item, this);
    }
    else
    {
        Q_UNIMPLEMENTED();
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
