#include "setlistwidget.h"
#include "ui_setlistwidget.h"
#include "application.h"
#include "Database/SongDatabase/songdatabase.h"
#include "project.h"
#include "DatabaseView/SongDatabaseView/songtableviewcontainer.h"
#include <QDialog>
#include <QDialogButtonBox>

SetlistWidget::SetlistWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetlistWidget),
    m_selector( new SetlistItemSelector( this ))
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

SetlistItem* SetlistWidget::currentItem() const
{
    if (!ui->listView->selectionModel() || !ui->listView->model())
    {
        // no model (i.e. no setlist) is set.
        return NULL;
    }
    QModelIndexList selection = ui->listView->selectionModel()->selectedRows();

    if (selection.isEmpty())
    {
        return NULL;
    }

    return qobject_assert_cast<Setlist*>( ui->listView->model() )->itemAt( selection.first() );

}

void SetlistWidget::on_buttonSortUp_clicked()
{

}

void SetlistWidget::on_buttonSortDown_clicked()
{

}

#include "Commands/SetlistCommands/setlistremoveitemcommand.h"
void SetlistWidget::on_buttonDelete_clicked()
{
    SetlistItem* ci = currentItem();
    if (m_currentSetlist && ci)
    {
        app().pushCommand( new SetlistRemoveItemCommand( m_currentSetlist, ci ) );
    }
    else
    {
        qWarning() << "Cannot add Item to no set list.";
    }
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

void SetlistWidget::on_buttonShowSongs_clicked()
{
    m_selector->show();
}

void SetlistWidget::hideEvent(QHideEvent *e)
{
    m_selectorIsVisible = m_selector->isVisible();
    m_selectorGeometry = m_selector->geometry();
    m_selectorPosition = m_selector->pos();

    m_selector->hide();
    QWidget::hideEvent(e);
}

void SetlistWidget::showEvent(QShowEvent *e)
{
    if (m_selectorIsVisible)
    {
        m_selector->show();
        m_selector->setGeometry( m_selectorGeometry );
        m_selector->move( m_selectorPosition );
    }
    QWidget::showEvent(e);
}
