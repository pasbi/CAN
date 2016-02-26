#ifndef MERGELISTWIDGETITEMWIDGET_H
#define MERGELISTWIDGETITEMWIDGET_H

#include <QWidget>
#include "mergeitem.h"

class QComboBox;
class QPushButton;

class MergeListWidgetItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MergeListWidgetItemWidget(MergeItemBase* mergeItem);

signals:
    void clicked(MergeItemBase*);
    void indexChanged(MergeItemBase::Action);

private:
    MergeItemBase* m_mergeItem;
    QComboBox* makeComboBox(MergeItemBase::Origin origin);
    QPushButton* makePushButton();


};

#endif // MERGELISTWIDGETITEMWIDGET_H
