#ifndef SONGATTRIBUTEDELEGATE_H
#define SONGATTRIBUTEDELEGATE_H

#include <QItemDelegate>

class SongDatabaseSortProxy;
class SongDatabase;
class SongTableView;
class SongAttributeDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit SongAttributeDelegate(SongTableView *parent = 0);

    SongTableView* parent() const;
    SongDatabaseSortProxy *proxyModel() const;
    SongDatabase* model() const;

    bool isEditing() const { return m_isEditing; }

protected:
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;

private:
    QString editorType(const QModelIndex &index) const;
    QVariant editorOptions(const QModelIndex & index) const;
    mutable bool m_isEditing = false;


};

#endif // SONGATTRIBUTEDELEGATE_H
