#ifndef DATABASEWIDGET_H
#define DATABASEWIDGET_H

#include <QWidget>
#include "databaseview.h"

class DatabaseWidgetBase : public QWidget
{
protected:
    explicit DatabaseWidgetBase(QWidget* databaseViewContainer, QWidget *secondWidget, QWidget* parent);

};

template<typename T> class DatabaseView;

template<typename T, typename SecondWidgetType>
class DatabaseWidget : public DatabaseWidgetBase
{
protected:
    /**
     * @brief DatabaseWidget
     * @param databaseViewContainer DatabaseWidget takes ownership
     * @param secondWidget DatabaseWidget takes ownership
     * @param parent
     */
    DatabaseWidget(DatabaseView<T>* databaseView, SecondWidgetType* secondWidget, QWidget* parent = 0) :
        DatabaseWidgetBase(databaseView, secondWidget, parent),
        m_databaseView(databaseView),
        m_secondWidget(secondWidget)
    {
    }

public:
    T* currentItem() const
    {
        return databaseView()->currentItem();
    }

    DatabaseView<T>* databaseView() const
    {
        return m_databaseView;
    }

private:
protected:
    DatabaseView<T>* m_databaseView;
    SecondWidgetType* m_secondWidget;
};

#endif // DATABASEWIDGET_H
