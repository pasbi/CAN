#ifndef FREEZETABLEVIEW_H
#define FREEZETABLEVIEW_H

#include <QTableView>

class FreezeTableView : public QTableView
{
    typedef QTableView Super;
    Q_OBJECT
public:
    explicit FreezeTableView(QWidget *parent = 0);
    ~FreezeTableView();
    void setModel(QAbstractItemModel *model);

    void freezeFirstColumn(bool freeze);
    void setSelectionMode(SelectionMode mode);
    void setSelectionBehavior(SelectionBehavior behavior);

protected:
    void resizeEvent(QResizeEvent *event);
    QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers);
    void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);


private:
    void updateFrozenTableGeometry();

private slots:
    void updateSectionWidth(int logicalIndex, int oldSize, int newSize);
    void updateSectionHeight(int logicalIndex, int oldSize, int newSize);

private:
    QTableView* m_frozenTableView;

};

#endif // FREEZETABLEVIEW_H
