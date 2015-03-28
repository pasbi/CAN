#include "datetableview.h"
#include <QHeaderView>
#include <QItemDelegate>
#include <QComboBox>
#include "global.h"
#include "Database/DateDatabase/datedatabase.h"

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
        const DateDatabase* database = qobject_cast<const DateDatabase*>(index.model());
        QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
        assert( comboBox );

        // first column:
        if (index.column() == 0)
        {
            for ( Date::Type type : Date::getInstancesOf_Type() )
            {
                comboBox->addItem( Date::getTranslatedNameOf_Type( type ) );
            }
        }
        comboBox->setCurrentIndex( database->data( index, Qt::EditRole ).toInt() );

        // other columns should not have combobox delegate.
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
    {
        QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
        DateDatabase* database = qobject_cast<DateDatabase*>(model);    // index->model() is const

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

DateTableView::DateTableView(QWidget *parent) :
    QTableView(parent)
{
    horizontalHeader()->hide();
    verticalHeader()->hide();

    setItemDelegateForColumn( 0, new TypeComboBoxDelegate() );
}

DateTableView::~DateTableView()
{
}
