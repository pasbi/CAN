#ifndef DATABASEEDITITEMCOMMAND_H
#define DATABASEEDITITEMCOMMAND_H

#include "Commands/modelcommand.h"
#include <QModelIndex>

class QAbstractItemModel;
class DatabaseEditCommand : public ModelCommand<QAbstractItemModel>
{
public:
    DatabaseEditCommand(  QAbstractItemModel* database,  const QModelIndex & index, const QVariant & newData );
    void undo();
    void redo();

private:
    const QModelIndex   m_index;
    const QVariant      m_newData;
    const QVariant      m_oldData;
};

#endif // DATABASEEDITITEMCOMMAND_H
