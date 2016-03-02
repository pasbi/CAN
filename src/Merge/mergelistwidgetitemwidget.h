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
    explicit MergeListWidgetItemWidget(MergeItem* mergeItem);

signals:
    void clicked();
    void indexChanged(MergeItem::Action);

private:
    MergeItem* m_mergeItem;
    QComboBox* makeComboBox(MergeItem::Type type);
    QPushButton* makePushButton();


};

#endif // MERGELISTWIDGETITEMWIDGET_H
