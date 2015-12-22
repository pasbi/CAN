//#include "texteditdelegate.h"
//#include "Commands/DatabaseCommands/databaseeditcommand.h"
//#include "application.h"

//TextEditDelegate::TextEditDelegate(QObject *parent) :
//    ItemDelegate(parent)
//{

//}

//void TextEditDelegate::setSpecificEditorData(TextDialog* dialog, const QModelIndex& index) const
//{
//    QVariant data = index.model()->data(index, Qt::EditRole);
//    dialog->setText(data.toString());
//}

//void TextEditDelegate::setSpecificModelData(TextDialog* dialog, QAbstractItemModel *database, const QModelIndex &index) const
//{
//    app().pushCommand( new DatabaseEditCommand(database, index, dialog->text()) );
//}
