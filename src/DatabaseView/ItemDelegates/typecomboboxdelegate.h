#ifndef TYPECOMBOBOXDELEGATE_H
#define TYPECOMBOBOXDELEGATE_H

#include "itemdelegate.h"
#include <QComboBox>
#include "global.h"
#include "application.h"
#include "Commands/DatabaseCommands/databaseeditcommand.h"

class DatabaseBase;
class TypeComboBoxDelegate : public ItemDelegate<QComboBox>
{
public:
    TypeComboBoxDelegate( const QStringList& types, QObject* parent = 0 ) :
        ItemDelegate<QComboBox>( parent ),
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

    void setModelData(QComboBox *editor, QAbstractItemModel *model, const QModelIndex &index) const
    {
        if (model->data(index, Qt::EditRole) != editor->currentIndex())
        {
            app().pushCommand( new DatabaseEditCommand( model, index, editor->currentIndex()) );
        }
    }


private:
    const QStringList m_types;

};


#endif // TYPECOMBOBOXDELEGATE_H

