#include "typecomboboxdelegate.h"

TypeComboBoxDelegate::TypeComboBoxDelegate( const QStringList& types, QObject* parent ) :
    ItemDelegate( parent ),
    m_types(types)
{
}

void TypeComboBoxDelegate::setSpecificEditorData(QComboBox *editor, const QModelIndex &index) const
{
    editor->addItems(m_types);
    editor->setCurrentIndex( index.model()->data( index, Qt::EditRole ).toInt() );
}

void TypeComboBoxDelegate::setSpecificModelData(QComboBox *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (model->data(index, Qt::EditRole) != editor->currentIndex())
    {
        app().pushCommand( new DatabaseEditCommand( model, index, editor->currentIndex()) );
    }
}
