#include "attachmentmergewidget.h"
#include <QLineEdit>

AttachmentMergeWidget::AttachmentMergeWidget(MergeItem* mergeItem, QWidget *parent) :
    AttachmentMergeWidgetBase(mergeItem, parent)
{
    m_masterEdit = new QLineEdit(this);
    m_slaveEdit = new QLineEdit(this);
    addEditorWidget(m_masterEdit, m_slaveEdit);
}

AttachmentMergeWidget::~AttachmentMergeWidget()
{
}


void AttachmentMergeWidget::apply()
{
    //TODO
}
