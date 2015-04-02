#include "setlistview.h"
#include <QMimeData>
#include "global.h"
#include "application.h"
#include "project.h"
#include "Commands/SetlistCommands/setlistinsertitemcommand.h"

SetlistView::SetlistView(QWidget *parent) :
    QListView(parent)
{
    setAcceptDrops(true);
    setDropIndicatorShown(true);
}

void SetlistView::dragEnterEvent(QDragEnterEvent *event)
{
    if (model() && event->mimeData()->hasFormat("CAN/songs"))
    {
        event->acceptProposedAction();
    }
}

void SetlistView::dragMoveEvent(QDragMoveEvent *e)
{
    if (model() && e->mimeData()->hasFormat("CAN/songs"))
    {
        e->acceptProposedAction();
    }
}



void SetlistView::dropEvent(QDropEvent *e)
{
    if (!model())
    {
        return;
    }

    int position = 0;
    int n = model()->rowCount(QModelIndex());
    QModelIndex i = indexAt(e->pos());
    if (i.isValid())
    {
        position = i.row() + 1;
    }
    else
    {
        position = n;
    }
    position = qMin( position, n );

    qDebug() << "position = " << position;
    qDebug() << "index = " << i;

    QList<qintptr> ptrs;
    QDataStream stream( e->mimeData()->data("CAN/songs") );
    stream >> ptrs;
    if (!ptrs.isEmpty())
    {
        QList<SetlistItem*> newItems;
        app().project()->beginMacro(tr("Drop Songs"));
        for (qintptr ptr : ptrs)
        {
            Song* song = (Song*)( ptr );
            newItems << new SetlistItem( song );
            app().project()->pushCommand( new SetlistInsertItemCommand( model(), position, newItems.last() ) );
        }

        model()->notifyDataChanged( model()->index(model()->indexOf(newItems.first()), 0),
                                    model()->index(model()->indexOf(newItems.last()),  0) );
        app().project()->endMacro();
    }


    e->accept();

}
