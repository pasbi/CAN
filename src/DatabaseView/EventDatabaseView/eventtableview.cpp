#include "eventtableview.h"

#include <QHeaderView>
#include <QItemDelegate>
#include <QComboBox>
#include <QMouseEvent>

#include "global.h"
#include "Database/EventDatabase/eventdatabase.h"
#include "Dialogs/datetimedialog.h"
#include "util.h"
#include "Database/EventDatabase/eventdatabasesortproxy.h"
#include "DatabaseView/ItemDelegates/typecomboboxdelegate.h"
#include "DatabaseView/ItemDelegates/lineeditdelegate.h"
#include "Commands/DatabaseCommands/databaseeditcommand.h"
#include "Database/database.h"
#include "application.h"
#include "Project/project.h"


EventTableView::EventTableView(QWidget *parent) :
    DatabaseView( new EventDatabaseSortProxy(), parent )
{
    setModel( app().project()->eventDatabase() );

    setItemDelegateForColumn( 0, new TypeComboBoxDelegate<Event::Type>( Event::eventTypeNames(), this ) );
    setItemDelegateForColumn( 2, new LineEditDelegate( this ));

    horizontalHeader()->setResizeContentsPrecision( -1 ); // look at all rows.
    horizontalHeader()->setSectionResizeMode( QHeaderView::ResizeToContents );

}

int EventTableView::sizeHintForColumn(int column) const
{
    int additional = 0;
    if (model())
    {
        int numCols = model()->columnCount();
        int currentWidth = 0;
        for (int i = 0; i < numCols; ++i)
        {
            currentWidth += QTableView::sizeHintForColumn( i );
        }

        additional = (viewport()->width() - currentWidth) / numCols;
    }
    return QTableView::sizeHintForColumn( column ) + qMax( additional, 10 );
}

bool EventTableView::showDateTimeDialog(QModelIndex index)
{
    if (index.column() == 1)
    {
        DateTimeDialog dialog;
        QDateTime currentDateTime = itemAtIndex(index)->attribute("beginning").toDateTime();
        dialog.setDateTime( currentDateTime );
        if (dialog.exec() == QDialog::Accepted)
        {
            pushCommand( new DatabaseEditCommand(model(), index, QVariant::fromValue(dialog.dateTime())));
        }
        return true;
    }
    return false;
}

void EventTableView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (!showDateTimeDialog( indexAt( event->pos() ) ))
    {
        QTableView::mouseDoubleClickEvent( event );
    }
}

void EventTableView::triggerEditDate(const Event *event)
{
    showDateTimeDialog( sourceModel()->indexOf(event, 1) );
}
