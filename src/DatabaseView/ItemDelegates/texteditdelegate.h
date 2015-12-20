#ifndef TEXTEDITDELEGATE_H
#define TEXTEDITDELEGATE_H

#include "itemdelegate.h"
#include <QTextEdit>

class TextEditDelegate : public ItemDelegate<QTextEdit>
{
public:
    TextEditDelegate(QObject* parent = nullptr);

private:
    void setEditorData(QTextEdit* editor, const QModelIndex& index) const;
    void setModelData(QTextEdit *editor, QAbstractItemModel *database, const QModelIndex &index) const;
};

#endif // TEXTEDITDELEGATE_H
