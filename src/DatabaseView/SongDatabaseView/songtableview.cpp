#include "songtableview.h"
#include "QHeaderView"
#include "global.h"
#include "Project/project.h"
#include "util.h"
#include <QFocusEvent>
#include "application.h"
#include "DatabaseView/ItemDelegates/durationdelegate.h"
#include "DatabaseView/ItemDelegates/lineeditdelegate.h"
#include "DatabaseView/ItemDelegates/editablecomboboxdelegate.h"
#include "Database/SongDatabase/songdatabasesortproxy.h"
#include "DatabaseView/ItemDelegates/keydelegate.h"
#include "DatabaseView/ItemDelegates/typecomboboxdelegate.h"
#include "Dialogs/peoplesdialog.h"
#include "DatabaseView/ItemDelegates/texteditdelegate.h"
#include <QHeaderView>
#include "Dialogs/textdialog.h"


SongTableView::SongTableView(QWidget *parent) :
    DatabaseView(new SongDatabaseSortProxy(), parent)
{
    setModel( app().project()->songDatabase() );

    setDragEnabled(true);

    setItemDelegateForColumn(0, new LineEditDelegate(this));            // title
    setItemDelegateForColumn(1, new EditableComboBoxDelegate(this));    // artist
    setItemDelegateForColumn(2, new DurationDelegate(this));            // duration
    setItemDelegateForColumn(3, new KeyDelegate( this ) );              // key
    setItemDelegateForColumn(4, new TypeComboBoxDelegate(Song::LABEL_NAMES, this));
    setItemDelegateForColumn(5, new TypeComboBoxDelegate(Song::STATE_NAMES, this));

    connect(this, &QTableView::doubleClicked, [this](const QModelIndex& index)
    {
        if (index.column() == 6 || index.column() == 7)
        {
            PeoplesDialog dialog( static_cast<SongDatabase*>(sourceModel())->peoples(),
                                  model()->data(index, Qt::EditRole).toStringList(), this);
            dialog.exec();
            app().pushCommand( new DatabaseEditCommand(model(), index, dialog.activePeoples()));
        }

        if (index.column() == 8)
        {
            TextDialog dialog(this);
            dialog.setText(model()->data(index, Qt::EditRole).toString());
            dialog.exec();
            app().pushCommand( new DatabaseEditCommand(model(), index, dialog.text()));
        }
    });

    verticalHeader()->show();
}
