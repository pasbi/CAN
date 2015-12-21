#include "keydelegate.h"
#include "chord.h"
#include "application.h"
#include "Commands/DatabaseCommands/databaseeditcommand.h"

KeyDelegate::KeyDelegate(QObject *parent) :
    ItemDelegate(parent)
{
}

void KeyDelegate::setEditorData(KeyEditor* editor, const QModelIndex& index) const
{
    editor->setKey( index.model()->data(index, Qt::EditRole).value<Chord>() );
}

void KeyDelegate::setModelData(KeyEditor *editor, QAbstractItemModel *database, const QModelIndex &index) const
{
    app().pushCommand(new DatabaseEditCommand(database, index, QVariant::fromValue(editor->key())));
}
