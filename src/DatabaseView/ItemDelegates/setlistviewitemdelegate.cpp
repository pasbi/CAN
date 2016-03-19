#include "setlistviewitemdelegate.h"
#include "Database/EventDatabase/setlist.h"

#include <QLineEdit>
#include "setlistitemcombobox.h"
#include "Database/EventDatabase/event.h"
#include "Database/SongDatabase/songdatabase.h"

#include "util.h"
#include "Commands/SetlistCommands/setlistitemchangesongcommand.h"
#include "Commands/DatabaseCommands/databaseeditcommand.h"
#include "Database/EventDatabase/setlistitem.h"
#include "Project/project.h"


#include <QTableView>

SetlistViewItemDelegate::SetlistViewItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget* SetlistViewItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_ASSERT(itemFromIndex(index));
    switch (itemFromIndex(index)->attribute("type").value<SetlistItem::Type>())
    {
    case SetlistItem::SongType:
    {
        SetlistItemComboBox* box = new SetlistItemComboBox(parent);
//        box->setView(new QTableView(box));
        return box;
    }
    case SetlistItem::LabelType:
        return new QLineEdit(parent);
    default:
        Q_ASSERT(false);
        return nullptr;
    }
}

void SetlistViewItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    SetlistItem* item = itemFromIndex(index);
    switch (item->attribute("type").value<SetlistItem::Type>())
    {
    case SetlistItem::SongType:
    {
        SetlistItemComboBox* comboBox = qobject_assert_cast<SetlistItemComboBox*>(editor);
        QList<Song*> availableSongs = item->database()->project()->songDatabase()->items();
        QStringList songLabels;
        for (const Song* song : availableSongs)
        {
            songLabels << song->label();
        }
        comboBox->addItems(songLabels);

        int index = availableSongs.indexOf(const_cast<Song*>(item->attribute("song").value<const Song*>()));
        comboBox->setCurrentIndex(index);
        comboBox->selectAll();
    }
        break;
    case SetlistItem::LabelType:
    {
        QLineEdit* lineEdit = qobject_assert_cast<QLineEdit*>(editor);
        lineEdit->setText(item->attribute("label").toString());
    }
        break;
    }
}

void SetlistViewItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    Q_UNUSED(model);
    SetlistItem* item = itemFromIndex(index);
    switch (item->attribute("type").value<SetlistItem::Type>())
    {
    case SetlistItem::SongType:
    {
        SetlistItemComboBox* comboBox = qobject_assert_cast<SetlistItemComboBox*>(editor);
        QList<Song*> availableSongs = item->database()->project()->songDatabase()->items();

        int comboBoxIndex = comboBox->currentIndex();
        if (comboBoxIndex >= 0)
        {
            const Song* song = availableSongs[comboBoxIndex];
            qDebug() << "set song" << song->attribute("title").toString() << song->attribute("artist").toString();
            if (item->attribute("song").value<const Song*>() != song)
            {
                pushCommand( new SetlistItemChangeSongCommand(item, song));
            }
        }
    }
        break;
    case SetlistItem::LabelType:
    {
        QLineEdit* lineEdit = qobject_assert_cast<QLineEdit*>(editor);
        QVariant newValue = lineEdit->text();
        if (model->data(index) != newValue)
        {
            pushCommand( new DatabaseEditCommand( model, index, newValue) );
        }
    }
        break;
    }
}

SetlistItem* SetlistViewItemDelegate::itemFromIndex(const QModelIndex &index)
{
    return index.model()->data(index, Qt::UserRole).value<SetlistItem*>();
}
