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
#include "application.h"
#include <QResizeEvent>


SongTableView::SongTableView(QWidget *parent) :
    DatabaseView(new SongDatabaseSortProxy(), parent),
    m_detailsShown(false)
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

    connect(horizontalHeader(), SIGNAL(sectionResized(int,int,int)), this, SLOT(updateColumnWidths()));
    restoreColumnWidths();
    updateColumnWidths();

}

void SongTableView::setDetailedView(bool detailed)
{
    storeColumnWidths();

    m_detailsShown = detailed;
    freezeFirstColumn(detailed);
    for (int i = 0; i < model()->columnCount(); ++i)
    {
        setColumnHidden(i, !detailed && i >= 2);
    }

    restoreColumnWidths();
}

void SongTableView::setHideInactives(bool hide)
{
    static_cast<SongDatabaseSortProxy*>(sortProxy())->setHideInactives(hide);
}

void SongTableView::resizeEvent(QResizeEvent *event)
{
    DatabaseView::resizeEvent(event);
    updateColumnWidths();
}

void SongTableView::updateColumnWidths()
{
    if (!m_detailsShown)
    {
        // assert there are 2 visible columns
        int width = viewport()->width() - 1;

        double left = qBound(20, columnWidth(0), width - 20);
        double right = qBound(20, columnWidth(1), width - 20);

        double t =  left / (left + right);

        setColumnWidth(0, t * width);
        setColumnWidth(1, (1-t) * width);

    }
}


void SongTableView::restoreColumnWidths()
{
    QList<int> widths = preference<QList<int>>("songDetailColumnWidths");
    if (m_detailsShown && !widths.isEmpty())
    {
        // restore header widths
        int n = QTableView::model() ? QTableView::model()->columnCount() : 0;
        for (int i = 0; i < n; ++i)
        {
            setColumnWidth(i, widths[i]);
        }
    }
}

void SongTableView::storeColumnWidths()
{
    if (m_detailsShown)
    {
        // store header widths
        int n = QTableView::model() ? QTableView::model()->columnCount() : 0;
        QList<int> widths;
        widths.reserve(n);
        for (int i = 0; i < n; ++i)
        {
            widths.append( horizontalHeader()->sectionSize(i) );
        }
        setPreference("songDetailColumnWidths", widths);
    }
}

void SongTableView::hideEvent(QHideEvent *event)
{
    storeColumnWidths();
    DatabaseView::hideEvent(event);
}
