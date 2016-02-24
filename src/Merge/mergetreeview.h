#ifndef MERGETREEVIEW_H
#define MERGETREEVIEW_H

#include <QWidget>
#include <QTreeWidgetItem>

namespace Ui {
class MergeTreeView;
}

template<class T> class MergeTreeItem;

class MergeTreeView : public QWidget
{
    Q_OBJECT

public:
    explicit MergeTreeView(QWidget *parent = 0);
    ~MergeTreeView();
    void setRootItem(QTreeWidgetItem* root);
    const QTreeWidgetItem* root() const { return m_root; }

protected:
    void resizeEvent(QResizeEvent *e);
    void showEvent(QShowEvent *e);

private:
    Ui::MergeTreeView *ui;
    void updateHeaderSize();
    QTreeWidgetItem* m_root;
};

#endif // MERGETREEVIEW_H
