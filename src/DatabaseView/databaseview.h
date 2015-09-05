#ifndef DATABASEVIEW_H
#define DATABASEVIEW_H

#include <QMenu>
#include <QTableView>

#include "Database/databasesortproxy.h"
#include "global.h"
#include <QKeyEvent>
#include "huddecorator.h"

class DatabaseViewBase : public QTableView
{
    Q_OBJECT
protected:
    DatabaseViewBase(QWidget* parent = 0);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *e);
    virtual void setFilter(const QString& filter);
    virtual QString filter() const = 0;
    void focusOutEvent(QFocusEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void leaveEvent(QEvent *event);
    void enterEvent(QEvent *event);

private:
    HudDecorator* m_hud;

signals:
    void clicked();
};

template<typename T>
class DatabaseView : public DatabaseViewBase
{
public:
    explicit DatabaseView(QWidget *parent = 0) :
        DatabaseViewBase(parent)
    {
        setAutoScroll(true);
        setContextMenuPolicy(Qt::ActionsContextMenu);
        setAlternatingRowColors( true );
        setEditTriggers( QAbstractItemView::DoubleClicked
                       | QAbstractItemView::EditKeyPressed );
    }

    QModelIndex indexUnderCursor() const
    {
        QPoint pos = viewport()->mapFromGlobal( QCursor::pos() );
        return indexAt(pos);
    }

    void setModel(DatabaseSortProxy<T> *model)
    {
        DatabaseViewBase::setModel( model );
        connect( model->sourceModel(), &QAbstractTableModel::rowsInserted, [this](QModelIndex, int last) {
            selectRow( last );
        });
    }

    DatabaseSortProxy<T>* proxyModel() const
    {
        // cast may fail, e.g. if model was set via QTableView::setModel
        return dynamic_cast<DatabaseSortProxy<T>*>(DatabaseViewBase::model());
    }

    Database<T>* model() const
    {
        QAbstractItemModel* model = DatabaseViewBase::model();
        while (model && model->inherits("QAbstractProxyModel"))
        {
            model = qobject_assert_cast<QAbstractProxyModel*>(model)->sourceModel();
        }

        return static_cast<Database<T>*>(model);
    }

    QString filter() const
    {
        if (proxyModel())
        {
            return proxyModel()->filter();
        }
        else
        {
            return QString();
        }
    }

    void setFilter(const QString &filter)
    {
        if (proxyModel())
        {
            proxyModel()->setFilter(filter);
        }
        DatabaseViewBase::setFilter(filter);
    }

    T* currentItem() const
    {
        QModelIndexList rows = selectionModel()->selectedRows();
        if (rows.isEmpty())
        {
            return nullptr;
        }
        else
        {
            return model()->resolveItemAtIndex( rows.first() );
        }
    }
};

#endif // DATABASEVIEW_H
