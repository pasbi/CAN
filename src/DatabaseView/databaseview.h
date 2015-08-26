#ifndef DATABASEVIEW_H
#define DATABASEVIEW_H

#include <QTableView>
#include "taggable.h"
#include "Database/databasesortproxy.h"
#include "databaseview.h"
#include <QMenu>
#include "Dialogs/tagdialog.h"
#include "application.h"
#include "Commands/edittagscommand.h"
#include "Database/database.h"

template<typename T>
class DatabaseView : public QTableView
{
public:
    explicit DatabaseView(QWidget *parent = 0) :
        QTableView(parent)
    {
        setContextMenuPolicy(Qt::ActionsContextMenu);
        setAlternatingRowColors( true );
        setEditTriggers( QAbstractItemView::DoubleClicked
                       | QAbstractItemView::EditKeyPressed );

        QAction* editTagsAction = new QAction( this );
        addAction( editTagsAction );
        editTagsAction->setText(tr("Edit Tags"));
        editTagsAction->setShortcut(QKeySequence("Ctrl+T"));
        editTagsAction->setIcon(QIcon(":/icons/icons/tag-2.png"));
        connect(editTagsAction, &QAction::triggered, [this]()
        {
            QModelIndexList list = selectionModel()->selectedIndexes();
            if (!list.isEmpty())
            {
                Taggable* t = model()->sourceModel()->itemAtIndex(list.first());
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

    QModelIndex indexUnderCursor() const
    {
        QPoint pos = viewport()->mapFromGlobal( QCursor::pos() );
        return indexAt(pos);
    }

    void setModel(DatabaseSortProxy<T> *model)
    {
        QTableView::setModel( model );
        connect( model->sourceModel(), &QAbstractTableModel::rowsInserted, [this](QModelIndex, int last) {
            selectRow( last );
        });
    }

    DatabaseSortProxy<T>* model() const
    {
        return static_cast<DatabaseSortProxy<T>*>(QTableView::model());
    }

};

#endif // DATABASEVIEW_H
