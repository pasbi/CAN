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


SongTableView::SongTableView(QWidget *parent) :
    DatabaseView(parent)
{
    setDragEnabled(true);

    setItemDelegateForColumn(0, new LineEditDelegate<Song>(this));
    setItemDelegateForColumn(1, new EditableComboBoxDelegate<Song>(this));
    setItemDelegateForColumn(3, new DurationDelegate<Song>(this));
}
