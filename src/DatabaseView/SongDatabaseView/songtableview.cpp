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
#include <QHeaderView>


SongTableView::SongTableView(QWidget *parent) :
    DatabaseView(new SongDatabaseSortProxy(), parent)
{
    setModel( app().project()->songDatabase() );

    setDragEnabled(true);

    setItemDelegateForColumn(0, new LineEditDelegate(this));
    setItemDelegateForColumn(1, new EditableComboBoxDelegate(this));
    setItemDelegateForColumn(2, new DurationDelegate(this));
}
