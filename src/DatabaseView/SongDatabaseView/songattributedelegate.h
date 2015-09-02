#ifndef SONGATTRIBUTEDELEGATE_H
#define SONGATTRIBUTEDELEGATE_H

#include <QItemDelegate>

template<typename T> class DatabaseSortProxy;
template<typename T> class Database;
class Song;
class SongTableView;
class SongAttributeDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit SongAttributeDelegate(SongTableView *parent = 0);

    SongTableView* parent() const;
    DatabaseSortProxy<Song> *proxyModel() const;
    Database<Song>* model() const;

    bool isEditing() const { return m_isEditing; }

protected:
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;

private:
    mutable bool m_isEditing = false;


};

#endif // SONGATTRIBUTEDELEGATE_H
