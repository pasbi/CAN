#ifndef SONGATTRIBUTEDELEGATE_H
#define SONGATTRIBUTEDELEGATE_H

#include <QItemDelegate>

class SongDatabase;
class SongTableView;
class SongAttributeDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit SongAttributeDelegate(SongTableView *parent = 0);

    SongTableView* parent() const;
    SongDatabase* model() const;

protected:
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;

private:
    QString editorType(const QModelIndex &index) const;
    QVariant editorOptions(const QModelIndex & index) const;

};

#endif // SONGATTRIBUTEDELEGATE_H
