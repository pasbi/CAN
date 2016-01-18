#include "eventtableview.h"
#include <QHeaderView>
#include <QItemDelegate>
#include <QComboBox>
#include "global.h"
#include "Database/EventDatabase/eventdatabase.h"
#include "Dialogs/datetimedialog.h"
#include <QMouseEvent>
#include "util.h"
#include "Database/EventDatabase/eventdatabasesortproxy.h"
#include "DatabaseView/ItemDelegates/typecomboboxdelegate.h"
#include "DatabaseView/ItemDelegates/lineeditdelegate.h"
#include "DatabaseView/ItemDelegates/lineeditdelegate.h"
#include "Commands/DatabaseCommands/databaseeditcommand.h"
#include "Database/database.h"


EventTableView::EventTableView(QWidget *parent) :
    DatabaseView( new EventDatabaseSortProxy(), parent )
{
    setModel( app().project()->eventDatabase() );

    setItemDelegateForColumn( 0, new TypeComboBoxDelegate( Event::eventTypeNames(), this ) );
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

bool EventTableView::showDialog(QModelIndex index)
{
    if (index.column() == 1)
    {
        DateTimeDialog dialog;
        dialog.setDateTime( model()->data( index, Qt::EditRole ).toDateTime() );
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
    if (!showDialog( indexAt( event->pos() ) ))
    {
        QTableView::mouseDoubleClickEvent( event );
    }
}
