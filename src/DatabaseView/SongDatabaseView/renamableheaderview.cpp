#include "renamableheaderview.h"
#include <QMouseEvent>
#include <QLineEdit>
#include "global.h"
#include "project.h"
#include "Commands/SongDatabaseCommands/songdatabaserenameheadercommand.h"
#include "Commands/SongDatabaseCommands/songdatabaseremovecolumncommand.h"
#include "util.h"
#include "global.h"
#include "application.h"
#include <QMessageBox>

RenamableHeaderView::RenamableHeaderView(Qt::Orientation orientation, SongTableView *parent) :
    QHeaderView(orientation, parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(this, SIGNAL(sectionResized(int,int,int)), parent, SLOT(fakeFocusOutEvent()));
}

void RenamableHeaderView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (!m_readOnly)
    {
        // leave a space around the resize handles
        int sectionLeft  = parent()->columnAt(event->pos().x() - 5);
        int sectionRight = parent()->columnAt(event->pos().x() + 5);

        if (sectionLeft == sectionRight)
            editHeader( sectionRight );
    }

    QHeaderView::mouseDoubleClickEvent( event );
}

void RenamableHeaderView::editHeader(int section, bool endMacroOnFinish)
{
    if (section < 0)
        return;

    int x = parent()->columnViewportPosition(section);
    int width = parent()->columnWidth(section);

    QLineEdit* lineEdit = new QLineEdit(this);
    lineEdit->setPlaceholderText(tr("Attribute Name"));
    lineEdit->setFixedSize(width, height() );
    lineEdit->move( x, 0 );
    lineEdit->setFocus();
    lineEdit->setText( parent()->model()->headerData( section, orientation(), Qt::EditRole ).toString() );
    lineEdit->show();

    connect(lineEdit, &QLineEdit::editingFinished, [this, endMacroOnFinish, lineEdit, section](){
        SongDatabase* database = parent()->model();
        app().pushCommand( new SongDatabaseRenameHeaderCommand( database, lineEdit->text(), section, orientation() ) );

        if (endMacroOnFinish)
        {
            database->project()->endMacro();
        }
        lineEdit->deleteLater();
    });
}

int RenamableHeaderView::sectionUnderCursor() const
{
    QPoint pos = viewport()->mapFromGlobal( QCursor::pos() );
    return logicalIndexAt(pos.x());
}

void RenamableHeaderView::showContextMenu(QPoint pos)
{
    QMenu* menu = new QMenu(this);

    setUpContextMenu(menu);

    menu->popup(viewport()->mapToGlobal(pos));
    connect(menu, SIGNAL(aboutToHide()), menu, SLOT(deleteLater()));
}

#include "Commands/SongDatabaseCommands/songdatabasenewattributecommand.h"
void RenamableHeaderView::setUpContextMenu(QMenu *menu)
{
    int section = sectionUnderCursor();
    if (section < 0)
        return;


    SongDatabase* database =  parent()->model();
    QString key = database->attributeKeys()[section];

    Util::addAction(menu, QString(tr("Delete Attribute %1")).arg(key), [this, database, section](){
        if (parent()->model()->allowDeleteColumn(section))
        {
            app().pushCommand( new SongDatabaseRemoveColumnCommand( database, section ) );
        }
        else
        {
            QMessageBox::information( this,
                                      tr("Cannot remove column"),
                                      tr("Predefined rows cannot be deleted. You may hide or rename it."),
                                      QMessageBox::Ok,
                                      QMessageBox::NoButton );
        }
    });

    Util::addAction(menu, tr("Add Attribute"), [this, database]() {
        SongDatabaseNewAttributeCommand* naCommand = new SongDatabaseNewAttributeCommand( database );

        // end macro is in renamableheaderview.cpp
        app().beginMacro( naCommand->text() );
        app().pushCommand( naCommand );
        editHeader( model()->columnCount() - 1, true );
    });
}

void RenamableHeaderView::setReadOnly()
{
    setContextMenuPolicy( Qt::NoContextMenu );
    m_readOnly = true;
}
