#ifndef DATABASEWIDGET_H
#define DATABASEWIDGET_H

#include <QWidget>
#include "databaseviewcontainer.h"

template<typename T> class DatabaseView;
template<typename T>
class DatabaseWidget : public QWidget
{
protected:
    DatabaseWidget(QWidget* parent = 0) :
        QWidget(parent)
    {
    }

public:
    T* currentItem() const
    {
        return m_databaseViewContainer->currentItem();
    }

    DatabaseView<T>* databaseView() const
    {
        return m_databaseViewContainer->databaseView();
    }

protected:
    DatabaseViewContainer<T>* m_databaseViewContainer;
};

#endif // DATABASEWIDGET_H
