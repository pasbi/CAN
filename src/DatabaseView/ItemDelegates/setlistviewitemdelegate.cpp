#include "setlistviewitemdelegate.h"
#include "Database/EventDatabase/setlist.h"
#include "Database/EventDatabase/setlist.h"

#include <QLineEdit>
#include <QComboBox>
#include "Database/EventDatabase/event.h"
#include "Database/SongDatabase/songdatabase.h"

#include "util.h"
#include "Commands/SetlistCommands/setlistitemchangesongcommand.h"
#include "Commands/DatabaseCommands/databaseedititemcommand.h"

SetlistViewItemDelegate::SetlistViewItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget* SetlistViewItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    if (item(index)->type() == SetlistItem::SongType)
    {
        return new QComboBox(parent);
    }
    else
    {
        return new QLineEdit(parent);
    }
}

void SetlistViewItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (item(index)->type() == SetlistItem::SongType)
    {
        QComboBox* comboBox = qobject_assert_cast<QComboBox*>(editor);
        QList<Song*> availableSongs = setlist(index)->event()->database()->project()->songDatabase()->items();
        QStringList songLabels;
        for (const Song* song : availableSongs)
        {
            songLabels << SetlistItem::labelSong(song);
        }
        comboBox->addItems(songLabels);
        comboBox->setEditable(true);
        comboBox->setInsertPolicy(QComboBox::NoInsert);

        const Song* song = item(index)->song();
        int index = indexOfConstInList<Song>( availableSongs, song );
        comboBox->setCurrentIndex(index);
        comboBox->lineEdit()->selectAll();
    }
    else // SetlistItem::LabelType
    {
        QLineEdit* lineEdit = qobject_assert_cast<QLineEdit*>(editor);
        lineEdit->setText(item(index)->label());
    }
}

void SetlistViewItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    Q_UNUSED(model);
    if (item(index)->type() == SetlistItem::SongType)
    {
        QComboBox* comboBox = qobject_assert_cast<QComboBox*>(editor);
        QList<Song*> availableSongs = setlist(index)->event()->database()->project()->songDatabase()->items();

        int comboBoxIndex = comboBox->currentIndex();
        if (comboBoxIndex >= 0)
        {
            const Song* song = availableSongs[comboBoxIndex];
            app().pushCommand( new SetlistItemChangeSongCommand(item(index), song));
        }
    }
    else // SetlistItem::LabelType
    {
        QLineEdit* lineEdit = qobject_assert_cast<QLineEdit*>(editor);
        app().pushCommand( new DatabaseEditItemCommand<SetlistItem>(item(index)->database(), index, lineEdit->text()) );
    }
}

const Setlist* SetlistViewItemDelegate::setlist(const QModelIndex &index)
{
    return qobject_assert_cast<const Setlist*>(index.model());

}

SetlistItem* SetlistViewItemDelegate::item(const QModelIndex &index)
{
    return setlist(index)->itemAtIndex(index);
}
