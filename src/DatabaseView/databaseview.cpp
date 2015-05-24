#include "databaseview.h"
#include <QMenu>
#include "Dialogs/tagdialog.h"
#include "application.h"
#include "Commands/edittagscommand.h"

DatabaseView::DatabaseView(QWidget *parent) :
    QTableView(parent)
{
    setContextMenuPolicy(Qt::ActionsContextMenu);
    setAlternatingRowColors( true );
    setEditTriggers( QAbstractItemView::DoubleClicked
                   | QAbstractItemView::EditKeyPressed );

    QAction* editTagsAction = new QAction( this );
    addAction( editTagsAction );
    editTagsAction->setText(tr("Edit Tags"));
    connect(editTagsAction, &QAction::triggered, [this]()
    {
        QModelIndexList list = selectionModel()->selectedIndexes();
        if (!list.isEmpty())
        {
            Taggable* t = objectAt(list.first());
            if (t != 0)
            {
                TagDialog d(t->tags(), this);
                if (d.exec() == QDialog::Accepted)
                {
                    app().pushCommand( new EditTagsCommand( t, d.tags() ) );
                }
            }
        }

    });
}

QModelIndex DatabaseView::indexUnderCursor() const
{
    QPoint pos = viewport()->mapFromGlobal( QCursor::pos() );
    return indexAt(pos);
}
