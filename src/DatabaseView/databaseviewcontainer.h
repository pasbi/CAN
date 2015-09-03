#ifndef DATABASEVIEWCONTAINER_H
#define DATABASEVIEWCONTAINER_H

#include "filterwidget.h"
#include "global.h"
#include <QModelIndex>
#include <QVBoxLayout>

class DatabaseViewContainerBase : public QWidget
{
protected:
    explicit DatabaseViewContainerBase(QWidget* databaseView, QWidget *parent);
    FilterWidget* m_filterWidget;
};

template<typename T> class DatabaseView;
template<typename T> class DatabaseSortProxy;
template<typename T>
class DatabaseViewContainer : public DatabaseViewContainerBase
{
protected:
    /**
     * @brief DatabaseViewContainer
     * @param databaseView DatabaseViewContainer takes the ownership of datbaseView
     * @param parent
     */
    explicit DatabaseViewContainer(DatabaseView<T>* databaseView, QWidget* parent = 0) :
        DatabaseViewContainerBase(databaseView, parent),
        m_databaseView(databaseView)
    {
    }

public:
    DatabaseView<T>* databaseView() const
    {
        return m_databaseView;
    }

    void setDatabase(DatabaseSortProxy<T>* database)
    {
        m_databaseView->setModel(database);
        connect(m_filterWidget, SIGNAL(filterChanged(QString)), database, SLOT(setFilter(QString)));
    }

    T* currentItem() const
    {
        QModelIndexList rows = m_databaseView->selectionModel()->selectedRows();
        if (rows.isEmpty())
        {
            return NULL;
        }
        else
        {
            return m_databaseView->model()->resolveItemAtIndex( rows.first() );
        }
    }

protected:
    DatabaseView<T>* m_databaseView;

};

#endif // DATABASEVIEWCONTAINER_H
