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
    void clicked(MergeItem*);

private:
    MergeItem* m_mergeItem;
    QWidget* makeButton();


};

#endif // MERGELISTWIDGETITEMWIDGET_H
