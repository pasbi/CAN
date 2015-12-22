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
    TypeComboBoxDelegate( const QStringList& types, QObject* parent = nullptr );

protected:
    void setSpecificEditorData(QComboBox *editor, const QModelIndex &index) const;
    void setSpecificModelData(QComboBox *editor, QAbstractItemModel *model, const QModelIndex &index) const;


private:
    const QStringList m_types;

};


#endif // TYPECOMBOBOXDELEGATE_H

