#include "songdatabasewidget.h"
#include <QTimer>
#include "Project/project.h"
#include "DatabaseView/SongDatabaseView/songtableview.h"
#include "Database/database.h"
#include <QSplitter>
#include "AttachmentView/attachmentchooser.h"
#include <QHBoxLayout>
#include "Database/SongDatabase/songdatabasesortproxy.h"
#include "Attachments/AudioAttachment/player.h"


SongDatabaseWidget::SongDatabaseWidget(QWidget *parent) :
    DatabaseWidget(new SongTableView(), new AttachmentChooser(), parent)
{
    connect( databaseView()->selectionModel(),
             SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
             this,
             SLOT(updateAttachmentChooser()) );

}

void SongDatabaseWidget::updateAttachmentChooser()
{
    Song* song = databaseView()->currentItem();
    attachmentChooser()->setSong( song );
    Player::stopActivePlayer();
}

AttachmentChooser* SongDatabaseWidget::attachmentChooser() const
{
    return m_secondWidget;
}
