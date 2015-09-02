#ifndef DATABASEVIEWCONTAINER_H
#define DATABASEVIEWCONTAINER_H

#include "filterwidget.h"
#include "global.h"
#include <QModelIndex>
#include <QVBoxLayout>


template<typename T> class DatabaseView;
template<typename T> class DatabaseSortProxy;

template<typename T>
class DatabaseViewContainer : public QWidget
{
protected:
    explicit DatabaseViewContainer(QWidget* parent = 0) :
        QWidget(parent)
    {
    }

    //TODO this code is without templates, consider to put it somewhere else
    void setupUi()
    {
        m_filterWidget = new FilterWidget(this);
        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(m_databaseView);
        layout->addWidget(m_filterWidget);
        setLayout(layout);
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
        qDebug() << (void*) m_databaseView;
        qDebug() << m_databaseView;
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
private:
    FilterWidget* m_filterWidget;
};

#endif // DATABASEVIEWCONTAINER_H
