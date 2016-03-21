#ifndef ATTACHMENTMERGEWIDGET_H
#define ATTACHMENTMERGEWIDGET_H

#include <QWidget>
#include "attachmentmergewidgetbase.h"

class QLineEdit;
class AttachmentMergeWidget : public AttachmentMergeWidgetBase
{
    Q_OBJECT

public:
    explicit AttachmentMergeWidget(MergeItem *mergeItem, QWidget *parent = 0);
    ~AttachmentMergeWidget();

private:
    QLineEdit* m_masterEdit;
    QLineEdit* m_slaveEdit;

};

#endif // ATTACHMENTMERGEWIDGET_H
