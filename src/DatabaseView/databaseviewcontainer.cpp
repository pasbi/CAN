#include "databaseviewcontainer.h"


DatabaseViewContainerBase::DatabaseViewContainerBase(QWidget *databaseView, QWidget *parent) :
    QWidget(parent)
{
    m_filterWidget = new FilterWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(databaseView);
    layout->addWidget(m_filterWidget);
    setLayout(layout);
}
