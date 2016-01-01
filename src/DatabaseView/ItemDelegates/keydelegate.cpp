#include "keydelegate.h"
#include "chord.h"
#include "Commands/DatabaseCommands/databaseeditcommand.h"

KeyDelegate::KeyDelegate(QObject *parent) :
    ItemDelegate(parent)
{
}

void KeyDelegate::setSpecificEditorData(KeyEditor* editor, const QModelIndex& index) const
{
    editor->setKey( index.model()->data(index, Qt::EditRole).value<Chord>() );
}

void KeyDelegate::setSpecificModelData(KeyEditor *editor, QAbstractItemModel *database, const QModelIndex &index) const
{
    pushCommand(new DatabaseEditCommand(database, index, QVariant::fromValue(editor->key())));
}
