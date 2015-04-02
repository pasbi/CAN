#include "setlistwidget.h"
#include "ui_setlistwidget.h"
#include "application.h"

SetlistWidget::SetlistWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetlistWidget)
{
    ui->setupUi(this);
}

SetlistWidget::~SetlistWidget()
{
    delete ui;
}

void SetlistWidget::setSetlist(Setlist *setlist)
{
    m_currentSetlist = setlist;
    ui->listView->setModel( setlist );
}

void SetlistWidget::on_buttonSortUp_clicked()
{

}

void SetlistWidget::on_buttonSortDown_clicked()
{

}

void SetlistWidget::on_buttonDelete_clicked()
{

}

#include "Commands/SetlistCommands/setlistadditemcommand.h"
void SetlistWidget::on_buttonAdd_clicked()
{
    if (m_currentSetlist)
    {
        app().pushCommand( new SetlistAddItemCommand( m_currentSetlist, new SetlistItem( QString("Hello") ) ) );
    }
    else
    {
        qWarning() << "Cannot add Item to no set list.";
    }
}

QListView* SetlistWidget::listView() const
{
    return ui->listView;
}
