#ifndef ATTACHMENTMERGEWIDGET_H
#define ATTACHMENTMERGEWIDGET_H

#include <QWidget>
#include "attachmentmergewidgetbase.h"

class QLineEdit;
class AttachmentMergeWidget : private AttachmentMergeWidgetBase
{
    Q_OBJECT

public:
    explicit AttachmentMergeWidget(MergeItem *mergeItem, QWidget *parent = 0);
    ~AttachmentMergeWidget();

protected:
    void apply();

private:
    QLineEdit* m_masterEdit;
    QLineEdit* m_slaveEdit;

};

#endif // ATTACHMENTMERGEWIDGET_H
