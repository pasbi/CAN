//#include "songattributedelegate.h"
//#include "songtableview.h"
//#include "CellEditors/celleditor.h"
//#include "global.h"
//#include "Database/SongDatabase/songdatabase.h"
//#include "Commands/DatabaseCommands/databaseedititemcommand.h"
//#include <QLineEdit>
//#include "Project/project.h"
//#include "application.h"
//#include "Database/SongDatabase/songdatabasesortproxy.h"

//SongAttributeDelegate::SongAttributeDelegate(SongTableView *parent) :
//    QItemDelegate(parent)
//{
//}

//#include "CellEditors/stringeditor.h"
//QWidget* SongAttributeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//    CellEditor* editor = NULL;

//    QString editorClassname;
//    switch (index.column())
//    {
//    case 0:
//        editorClassname = "StringEditor";
//        break;
//    case 1:
//        editorClassname = "ComboEditor";
//        break;
//    case 2:
//        qWarning() << "Request editor for read-only data";
//        editorClassname = "";
//        break;
//    case 3:
//        editorClassname = "DurationEditor";
//        break;
//    }

//    editor = Creatable::create<CellEditor>( editorClassname );
//    assert(editor);

//    editor->setParent(parent);
//    editor->setStyleOption(option);
//    editor->setIndex(index);
//    editor->setModel(SongAttributeDelegate::parent()->proxyModel());
//    editor->setCurrentData( proxyModel()->data( index, Qt::EditRole ) );
//    editor->polish();
//    editor->setFocus();

//    return editor;
//}

//Database<Song>* SongAttributeDelegate::model() const
//{
//    return parent()->model();
//}

//DatabaseSortProxy<Song>* SongAttributeDelegate::proxyModel() const
//{
//    return parent()->proxyModel();
//}

//SongTableView* SongAttributeDelegate::parent() const
//{
//    return qobject_assert_cast<SongTableView*>( QItemDelegate::parent() );
//}

//void SongAttributeDelegate::setModelData(QWidget *editor, QAbstractItemModel *, const QModelIndex &index) const
//{
//    QVariant newData;
//    if (!editor->inherits(CellEditor::staticMetaObject.className()))
//    {
//        WARNING << editor << " is not a CellEditor.";
//    }
//    else
//    {
//        newData = qobject_assert_cast<CellEditor*>(editor)->editedData();
//    }

//    if ( model()->data( index, Qt::EditRole ) != newData  )
//    {
//        app().pushCommand( new DatabaseEditItemCommand<Song>( model(), proxyModel()->mapToSource(index), newData ) );
//    }
//    m_isEditing = false;
//}

//void SongAttributeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
//{
//    // since editor gather informations from model itself, this mehod should do nothing.
//    Q_UNUSED(editor);
//    Q_UNUSED(index);
//    m_isEditing = true;
//}


