#ifndef TYPECOMBOBOXDELEGATE_H
#define TYPECOMBOBOXDELEGATE_H

#include "itemdelegate.h"
#include <QComboBox>
#include "global.h"
#include "Commands/DatabaseCommands/databaseeditcommand.h"

class DatabaseBase;
template<typename EnumType>
class TypeComboBoxDelegate : public ItemDelegate<QComboBox>
{
public:
    TypeComboBoxDelegate( const QStringList& types, QObject* parent = nullptr ) :
        ItemDelegate( parent ),
        m_types(types)
    {

    }

protected:
    void setSpecificEditorData(QComboBox *editor, const QModelIndex &index) const
    {
        editor->addItems(m_types);
        editor->setCurrentIndex( index.model()->data( index, Qt::EditRole ).value<EnumType>() );
    }

    void setSpecificModelData(QComboBox *editor, QAbstractItemModel *model, const QModelIndex &index) const
    {
        if (model->data(index, Qt::EditRole) != editor->currentIndex())
        {
            pushCommand( new DatabaseEditCommand( model, index, QVariant::fromValue(static_cast<EnumType>(editor->currentIndex())) ) );
        }
    }


private:
    const QStringList m_types;

};

#endif // TYPECOMBOBOXDELEGATE_H

