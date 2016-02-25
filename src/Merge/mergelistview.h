#ifndef MERGELISTVIEW_H
#define MERGELISTVIEW_H

#include <QWidget>
#include "mergeitem.h"


namespace Ui {
class MergeListView;
}

class MergeItemBase;

class MergeListView : public QWidget
{
    Q_OBJECT

public:
    explicit MergeListView(QWidget *parent = 0);
    ~MergeListView();
    QList<MergeItemBase> items() const { return m_items; }
    void setItems(const QList<MergeItemBase> items);

private:
    Ui::MergeListView *ui;
    void updateHeaderSize();
    QList<MergeItemBase> m_items;
};

#endif // MERGELISTVIEW_H
