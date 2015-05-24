#include "eventtableview.h"
#include <QHeaderView>
#include <QItemDelegate>
#include <QComboBox>
#include "global.h"
#include "Database/EventDatabase/eventdatabase.h"
#include "Dialogs/datetimedialog.h"
#include <QMouseEvent>
#include "util.h"
#include "application.h"

class TypeComboBoxDelegate : public QItemDelegate
{
public:
    TypeComboBoxDelegate( QObject* parent = NULL ) :
        QItemDelegate( parent )
    {
    }

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        Q_UNUSED( option );
        Q_UNUSED( index );
        return new QComboBox( parent );
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        const EventDatabaseSortProxy* database = qobject_assert_cast<const EventDatabaseSortProxy*>(index.model());
        QComboBox* comboBox = qobject_assert_cast<QComboBox*>(editor);
        assert( comboBox );

        // first column:
        if (index.column() == 0)
        {
            for ( Event::Type type : Event::getInstancesOf_Type() )
            {
                comboBox->addItem( Event::getTranslatedNameOf_Type( type ) );
            }
        }
        comboBox->setCurrentIndex( database->data( index, Qt::EditRole ).toInt() );

        // other columns should not have combobox delegate.
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
    {
        QComboBox* comboBox = qobject_assert_cast<QComboBox*>(editor);
        EventDatabaseSortProxy* database = qobject_assert_cast<EventDatabaseSortProxy*>(model);    // index->model() is const

        assert( database );
        assert( comboBox );

        if (index.column() == 0)
        {
            database->setData( index, comboBox->currentIndex(), Qt::EditRole );
        }
    }

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        Q_UNUSED( index );
        editor->setGeometry( option.rect );
    }
};

class DateTimeDelegate : public QItemDelegate
{
public:
    DateTimeDelegate( QObject* parent = NULL ) :
        QItemDelegate( parent )
    {

    }

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        Q_UNUSED( option );
        Q_UNUSED( index  );
        Q_UNUSED( parent );
        return new DateTimeDialog( NULL );
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        const EventDatabase* database = qobject_assert_cast<const EventDatabase*>(index.model());
        DateTimeDialog* dialog = qobject_assert_cast<DateTimeDialog*>(editor);
        assert( dialog );

        dialog->setTimeSpan( database->data( index, Qt::EditRole ).value<TimeSpan>() );
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
    {
        DateTimeDialog* dialog = qobject_assert_cast<DateTimeDialog*>(editor);
        EventDatabase* database = qobject_assert_cast<EventDatabase*>(model);    // index->model() is const

        assert( database );
        assert( dialog );

        if (index.column() == 0)
        {
            database->setData( index, QVariant::fromValue(dialog->timeSpan()), Qt::EditRole );
        }
    }

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        Q_UNUSED( index );
        editor->setGeometry( option.rect );
    }
};

EventTableView::EventTableView(QWidget *parent) :
    DatabaseView(parent)
{
    horizontalHeader()->setSectionResizeMode( QHeaderView::ResizeToContents );
    horizontalHeader()->setResizeContentsPrecision( -1 ); // look at all rows.
    horizontalHeader()->hide();

    setItemDelegateForColumn( 0, new TypeComboBoxDelegate( this ) );

    setSelectionBehavior( QAbstractItemView::SelectRows );
    setSelectionMode( QAbstractItemView::SingleSelection );

    verticalHeader()->hide();

    horizontalHeader()->setSectionsMovable( true );
    horizontalHeader()->setDragEnabled( true );
    horizontalHeader()->setDragDropMode( QAbstractItemView::InternalMove );

    setDragDropMode( QAbstractItemView::DragDrop );
    setDragEnabled( true );
    setDropIndicatorShown( true );
}

EventTableView::~EventTableView()
{
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

// returns whether a dialog was spawned
bool EventTableView::showDialog(QModelIndex index)
{
    if (index.column() == 1)
    {
        DateTimeDialog dialog;
        dialog.setTimeSpan( model()->data( index, Qt::EditRole ).value<TimeSpan>() );

        if (dialog.exec() == QDialog::Accepted)
        {
            model()->setData( index, QVariant::fromValue(dialog.timeSpan()), Qt::EditRole );
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

void EventTableView::setModel(EventDatabaseSortProxy *model)
{
    DatabaseView::setModel( model );
}

EventDatabase* EventTableView::model() const
{
    return proxyModel() ? proxyModel()->sourceModel() : NULL;
}

EventDatabaseSortProxy* EventTableView::proxyModel() const
{
    EventDatabaseSortProxy* pm = qobject_assert_cast<EventDatabaseSortProxy*>( QTableView::model() );
    return pm;
}

Qt::DropAction EventTableView::dropAction( QDropEvent* event )
{
    switch (event->keyboardModifiers())
    {
    case Qt::ControlModifier:
        return Qt::CopyAction;
    default:
        return Qt::MoveAction;
    }
}

void EventTableView::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat(EventDatabase::EVENT_POINTERS_MIME_DATA_FORMAT) )
    {
        event->setDropAction( dropAction( event ) );
        event->accept();
    }
}

void EventTableView::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat(EventDatabase::EVENT_POINTERS_MIME_DATA_FORMAT) )
    {
        event->setDropAction( dropAction( event ) );
        event->accept();
    }
}

#include "Commands/EventDatabaseCommands/eventdatabaseremoveeventcommand.h"
void EventTableView::dropEvent(QDropEvent *event)
{
    int row;
    QModelIndex index = indexAt( event->pos() );
    if (index.isValid())
    {
        row = proxyModel()->mapToSource( index ).row();
    }
    else
    {
        row = model()->rowCount();
    }

    pasteEvents( event->mimeData(), row, dropAction( event ) );
}

#include "Commands/EventDatabaseCommands/eventdatabasemoveeventcommand.h"
#include "Commands/EventDatabaseCommands/eventdatabaseneweventcommand.h"
void EventTableView::pasteEvents(const QMimeData *mimeData, int row, Qt::DropAction action)
{
    QByteArray data = mimeData->data( EventDatabase::EVENT_POINTERS_MIME_DATA_FORMAT );
    if (data.isEmpty())
    {
        return;
    }

    QList<qintptr> originalSongs;
    QDataStream stream( data );
    stream >> originalSongs;

    app().beginMacro( tr("Paste events") );
    if (action == Qt::MoveAction)
    {
        for (qintptr song : originalSongs)
        {
            QModelIndex index = model()->indexOfEvent((Event*) song);
            index = proxyModel()->mapFromSource( index );
            int sourceRow = proxyModel()->mapToSource( index ).row();

            app().pushCommand( new EventDatabaseMoveEventCommand( model(), sourceRow, row ) );
        }
    }
    else if (action == Qt::CopyAction)
    {
        for (qintptr song : originalSongs)
        {
            app().pushCommand( new EventDatabaseNewEventCommand( model(), ((Event*) song)->copy() ) );
        }
    }
    app().endMacro();
}

Taggable* EventTableView::objectAt(const QModelIndex &index)
{
    return model()->eventAtIndex( proxyModel()->mapToSource(index) );
}


