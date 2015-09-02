#ifndef DATABASEWIDGET_H
#define DATABASEWIDGET_H

#include <QWidget>

template<typename T> class DatabaseView;
template<typename T>
class DatabaseWidget : QWidget
{
protected:
    DatabaseWidget(QWidget* parent = 0) :
        QWidget(parent)
    {

    }

public:
    T* currentItem() const
    {

        return nullptr;
    }

    DatabaseView<T>* databaseView() const
    {
        return nullptr;
    }

};

#endif // DATABASEWIDGET_H
