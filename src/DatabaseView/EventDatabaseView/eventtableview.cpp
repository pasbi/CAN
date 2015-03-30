#include "eventtableview.h"
#include <QHeaderView>
#include <QItemDelegate>
#include <QComboBox>
#include "global.h"
#include "Database/EventDatabase/eventdatabase.h"
#include "Dialogs/datetimedialog.h"
#include <QMouseEvent>

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
        const EventDatabase* database = qobject_cast<const EventDatabase*>(index.model());
        QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
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
        QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
        EventDatabase* database = qobject_cast<EventDatabase*>(model);    // index->model() is const

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
        const EventDatabase* database = qobject_cast<const EventDatabase*>(index.model());
        DateTimeDialog* dialog = qobject_cast<DateTimeDialog*>(editor);
        assert( dialog );

        dialog->setDateTime( database->data( index, Qt::EditRole ).toDateTime() );
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
    {
        DateTimeDialog* dialog = qobject_cast<DateTimeDialog*>(editor);
        EventDatabase* database = qobject_cast<EventDatabase*>(model);    // index->model() is const

        assert( database );
        assert( dialog );

        if (index.column() == 0)
        {
            database->setData( index, dialog->dateTime(), Qt::EditRole );
        }
    }

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        Q_UNUSED( index );
        editor->setGeometry( option.rect );
    }
};

EventTableView::EventTableView(QWidget *parent) :
    QTableView(parent)
{
    horizontalHeader()->hide();
    horizontalHeader()->setSectionResizeMode( QHeaderView::ResizeToContents );
    horizontalHeader()->setResizeContentsPrecision( -1 ); // look at all rows.
    verticalHeader()->hide();


    setItemDelegateForColumn( 0, new TypeComboBoxDelegate( this ) );
    setSelectionBehavior( QAbstractItemView::SelectRows );
    // this does not work properly. Do it with events.
    // connect( this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(showDialog(QModelIndex)) );
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
        dialog.setDateTime( model()->data( index, Qt::EditRole ).toDateTime() );

        if (dialog.exec() == QDialog::Accepted)
        {
            model()->setData( index, dialog.dateTime(), Qt::EditRole );
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

void EventTableView::setModel(EventDatabase *model)
{
    QTableView::setModel( model );
}

EventDatabase* EventTableView::model() const
{
    EventDatabase* db = qobject_cast<EventDatabase*>( QTableView::model() );
    assert( db == QTableView::model() );    // may be NULL if model() itself is NULL
    return db;
}
