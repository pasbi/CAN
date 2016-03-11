#include "combineattachmentsdialog.h"

#include <QHBoxLayout>

#include "Merge/mergeitem.h"
#include "chordpatternattachmentmergewidget.h"

CombineAttachmentsDialog::CombineAttachmentsDialog(MergeItem* item, QWidget *parent) :
    QDialog(parent)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    if (item->inherits({"ChordPatternAttachment"}))
    {
        layout->addWidget(new ChordPatternAttachmentMergeWidget(item, this));
    }
    else
    {
        Q_UNIMPLEMENTED();
    }
    setLayout(layout);
}
