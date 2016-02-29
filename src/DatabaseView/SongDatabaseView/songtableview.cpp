#include "songtableview.h"
#include "QHeaderView"
#include "global.h"
#include "Project/project.h"
#include "util.h"
#include <QFocusEvent>
#include "DatabaseView/ItemDelegates/durationdelegate.h"
#include "DatabaseView/ItemDelegates/lineeditdelegate.h"
#include "DatabaseView/ItemDelegates/editablecomboboxdelegate.h"
#include "Database/SongDatabase/songdatabasesortproxy.h"
#include "DatabaseView/ItemDelegates/keydelegate.h"
#include "DatabaseView/ItemDelegates/typecomboboxdelegate.h"
#include "Dialogs/peoplesdialog.h"
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
    setItemDelegateForColumn(4, new TypeComboBoxDelegate<Song::Label>(Song::labelNames(), this));
    setItemDelegateForColumn(5, new TypeComboBoxDelegate<Song::State>(Song::stateNames(), this));

    connect(this, &QTableView::doubleClicked, [this](const QModelIndex& index)
    {
        if (index.column() == 6 || index.column() == 7)
        {
            PeoplesDialog dialog( static_cast<SongDatabase*>(sourceModel())->peoples(),
                                  model()->data(index, Qt::EditRole).toStringList(), this);
            dialog.exec();
            pushCommand( new DatabaseEditCommand(model(), index, dialog.activePeoples()));
        }

        if (index.column() == 8)
        {
            TextDialog dialog(this);
            dialog.setText(model()->data(index, Qt::EditRole).toString());
            dialog.exec();
            pushCommand( new DatabaseEditCommand(model(), index, dialog.text()));
        }
    });

    verticalHeader()->show();

}

void SongTableView::setDetailedView(bool detailed)
{
    freezeFirstColumn(detailed);
    for (int i = 0; i < model()->columnCount(); ++i)
    {
        setColumnHidden(i, !detailed && i >= 2);
    }
}

void SongTableView::setHideInactives(bool hide)
{
    static_cast<SongDatabaseSortProxy*>(sortProxy())->setHideInactives(hide);
}
