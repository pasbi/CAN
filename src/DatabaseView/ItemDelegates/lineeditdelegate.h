#ifndef LINEEDITDELEGATE_H
#define LINEEDITDELEGATE_H

#include <QLineEdit>

#include "itemdelegate.h"
#include "Commands/DatabaseCommands/databaseeditcommand.h"



class LineEditDelegate : public ItemDelegate<QLineEdit>
{
public:
    explicit LineEditDelegate(QObject *parent = nullptr);

protected:
    void setSpecificEditorData(QLineEdit *editor, const QModelIndex &index) const;
    void setSpecificModelData(QLineEdit *editor, QAbstractItemModel *model, const QModelIndex &index) const;
};

#endif // LINEEDITDELEGATE_H
