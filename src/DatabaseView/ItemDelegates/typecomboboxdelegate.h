#ifndef TYPECOMBOBOXDELEGATE_H
#define TYPECOMBOBOXDELEGATE_H

#include "itemdelegate.h"
#include <QComboBox>
#include "global.h"
#include "application.h"
#include "Commands/DatabaseCommands/databaseedititemcommand.h"

template<typename T>
class TypeComboBoxDelegate : public ItemDelegate<T, QComboBox>
{
public:
    TypeComboBoxDelegate( const QStringList& types, QObject* parent = 0 ) :
        ItemDelegate<T, QComboBox>( parent ),
        m_types(types)
    {
    }

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        Q_UNUSED( option );
        Q_UNUSED( index );
        return new QComboBox( parent );
    }

    void setEditorData(QComboBox *editor, const QModelIndex &index) const
    {
        editor->addItems(m_types);
        editor->setCurrentIndex( index.model()->data( index, Qt::EditRole ).toInt() );
    }

    void setModelData(QComboBox *editor, Database<T> *database, const QModelIndex &index) const
    {
        app().pushCommand( new DatabaseEditItemCommand<T>( database, index, editor->currentIndex(), Qt::EditRole) );
    }


private:
    const QStringList m_types;

};


#endif // TYPECOMBOBOXDELEGATE_H

