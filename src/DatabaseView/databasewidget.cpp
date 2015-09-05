#include "databasewidget.h"
#include <QSplitter>
#include <QHBoxLayout>

DatabaseWidgetBase::DatabaseWidgetBase(QWidget* databaseViewContainer, QWidget* secondWidget, QWidget *parent) :
    QWidget(parent)
{
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(databaseViewContainer);
    splitter->addWidget(secondWidget);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(splitter);
    setLayout(layout);
}
