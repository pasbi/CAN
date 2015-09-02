#include "typecomboboxdelegate.h"
#include <QComboBox>
#include "global.h"
#include "application.h"
#include "Commands/DatabaseCommands/databaseedititemcommand.h"
#include "Database/EventDatabase/event.h"
#include "Database/database.h"

TypeComboBoxDelegate::TypeComboBoxDelegate( QObject* parent ) :
    QItemDelegate( parent )
{
}

QWidget* TypeComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED( option );
    Q_UNUSED( index );
    return new QComboBox( parent );
}

//TODO do we need the very special class EvenTDatabaseSortProxy or is a base class sufficient?
void TypeComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox* comboBox = qobject_assert_cast<QComboBox*>(editor);
    assert( comboBox );

    // first column:
    if (index.column() == 0)
    {
        // fancy smart enums does not work because microsoft compiler does not support
        // variadic macros :(
        QStringList eventTypes = QStringList( { tr("Rehearsal"), tr("Gig"), tr("Other") } );
        for ( const QString& type : eventTypes )
        {
            comboBox->addItem( type );
        }
    }
    comboBox->setCurrentIndex( index.model()->data( index, Qt::EditRole ).toInt() );

    // other columns should not have combobox delegate.
}

void TypeComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox* comboBox = qobject_assert_cast<QComboBox*>(editor);
    assert( comboBox );

    if (index.column() == 0)
    {
        app().pushCommand( new DatabaseEditItemCommand<Event>( static_cast<Database<Event>*>(model), index, comboBox->currentIndex(), Qt::EditRole) );
    }
}

void TypeComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED( index );
    editor->setGeometry( option.rect );
}
