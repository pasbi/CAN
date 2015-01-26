#include "renamableheaderview.h"
#include <QMouseEvent>
#include <QLineEdit>
#include "global.h"
#include "project.h"
#include "Commands/SongDatabaseCommands/songdatabaserenameheadercommand.h"
#include "Commands/SongDatabaseCommands/songdatabaseremovecolumncommand.h"
#include <QAction>
#include <QMenu>
#include "util.h"
#include "global.h"
#include <QApplication>

RenamableHeaderView::RenamableHeaderView(Qt::Orientation orientation, SongTableView *parent) :
    QHeaderView(orientation, parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(this, &RenamableHeaderView::sectionResized, [this, parent](){
        // oddly, the columns in the table are not resized when the header is resized.
        // though, the colmns are resized when table loses focus.
        // since table can lose focus safely on resize, fire appropriate event to reach the
        // desired behaviour on the wrong track.
        QFocusEvent myFocusOutEvent(QEvent::FocusOut);
        QApplication::sendEvent( parent, &myFocusOutEvent );
    });
}

void RenamableHeaderView::mouseDoubleClickEvent(QMouseEvent *event)
{
    // leave a space around the resize handles
    int sectionLeft  = parent()->columnAt(event->pos().x() - 5);
    int sectionRight = parent()->columnAt(event->pos().x() + 5);

    if (sectionLeft == sectionRight)
        editHeader( sectionRight );
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
        database->project()->pushCommand( new SongDatabaseRenameHeaderCommand( database, lineEdit->text(), section, orientation() ) );

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

void RenamableHeaderView::setUpContextMenu(QMenu *menu)
{
    int section = sectionUnderCursor();
    if (section < 0)
        return;


    SongDatabase* database =  parent()->model();
    QString key = database->attributeKeys()[section];

    Util::addAction(menu, QString(tr("Delete Attribute %1")).arg(key), [this, database, section](){
        database->project()->pushCommand( new SongDatabaseRemoveColumnCommand( database, section ) );
    });
}
