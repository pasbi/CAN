#ifndef DATABASEWIDGET_H
#define DATABASEWIDGET_H

#include <QWidget>
#include "databaseviewcontainer.h"

class DatabaseWidgetBase : public QWidget
{
protected:
    explicit DatabaseWidgetBase(QWidget* databaseViewContainer, QWidget *secondWidget, QWidget* parent);

};

template<typename T> class DatabaseView;

template<typename DatabaseViewContainerType, typename SecondWidgetType>
class DatabaseWidget : public DatabaseWidgetBase
{
protected:
    /**
     * @brief DatabaseWidget
     * @param databaseViewContainer DatabaseWidget takes ownership
     * @param secondWidget DatabaseWidget takes ownership
     * @param parent
     */
    DatabaseWidget(DatabaseViewContainer<DatabaseViewContainerType>* databaseViewContainer, SecondWidgetType* secondWidget, QWidget* parent = 0) :
        DatabaseWidgetBase(databaseViewContainer, secondWidget, parent),
        m_databaseViewContainer(databaseViewContainer),
        m_secondWidget(secondWidget)
    {
    }

public:
    DatabaseViewContainerType* currentItem() const
    {
        return m_databaseViewContainer->currentItem();
    }

    DatabaseView<DatabaseViewContainerType>* databaseView() const
    {
        return m_databaseViewContainer->databaseView();
    }


protected:
    DatabaseViewContainer<DatabaseViewContainerType>* m_databaseViewContainer;
    SecondWidgetType* m_secondWidget;
};

#endif // DATABASEWIDGET_H
