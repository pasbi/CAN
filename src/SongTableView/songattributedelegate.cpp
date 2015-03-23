#include "songattributedelegate.h"
#include "songtableview.h"
#include "CellEditors/celleditor.h"
#include "global.h"
#include "Database/SongDatabase/songdatabase.h"
#include "Commands/SongDatabaseCommands/songdatabaseeditsongcommand.h"
#include <QLineEdit>
#include "project.h"

SongAttributeDelegate::SongAttributeDelegate(SongTableView *parent) :
    QItemDelegate(parent)
{
}

QWidget* SongAttributeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    CellEditor* editor = NULL;
    QString classname = QString("%1Editor").arg(editorType(index));

    if (Creatable::category(classname) != "CellEditor")
    {
        if (classname != "Editor")
        {
            // default to StringEditor without warning
            WARNING << "Unknown editor widget \"" << classname << "\".";
        }
        classname = "StringEditor";
    }
    assert( CREATE(classname, editor) );

    editor->setParent(parent);
    editor->setStyleOption(option);
    editor->setIndex(index);
    editor->setModel(SongAttributeDelegate::parent()->model());
    editor->setCurrentData( model()->data( index, Qt::EditRole ) );
    editor->polish();
    editor->setFocus();

    return editor;
}

SongDatabase* SongAttributeDelegate::model() const
{
    return parent()->model();
}

QString SongAttributeDelegate::editorType(const QModelIndex & index) const
{
    return parent()->model()->editorType(index);
}

SongTableView* SongAttributeDelegate::parent() const
{
    return qobject_cast<SongTableView*>( QItemDelegate::parent() );
}

void SongAttributeDelegate::setModelData(QWidget *editor, QAbstractItemModel *, const QModelIndex &index) const
{
    QVariant newData;
    if (!editor->inherits(CellEditor::staticMetaObject.className()))
    {
        WARNING << editor << " is not a CellEditor.";
    }
    else
    {
        newData = qobject_cast<CellEditor*>(editor)->editedData();
    }

    model()->project()->pushCommand( new SongDatabaseEditSongCommand( model(), index, newData, Qt::EditRole ) );
}

void SongAttributeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    // since editor gather informations from model itself, this mehod should do nothing.
    Q_UNUSED(editor);
    Q_UNUSED(index);
}


