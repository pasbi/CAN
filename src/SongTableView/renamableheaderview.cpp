#include "renamableheaderview.h"
#include <QMouseEvent>
#include <QLineEdit>
#include "global.h"
#include "project.h"
#include "Commands/SongDatabaseCommands/songdatabaserenameheadercommand.h"

RenamableHeaderView::RenamableHeaderView(Qt::Orientation orientation, SongTableView *parent) :
    QHeaderView(orientation, parent)
{
}

void RenamableHeaderView::mouseDoubleClickEvent(QMouseEvent *event)
{
    int section = parent()->columnAt(event->pos().x());

    editHeader( section );

    //spawnHeaderEdit();
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
    lineEdit->setText( parent()->model()->headerData( section, orientation(), Qt::DisplayRole ).toString() );
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

