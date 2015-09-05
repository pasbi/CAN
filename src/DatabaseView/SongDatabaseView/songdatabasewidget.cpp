#include "songdatabasewidget.h"
#include <QTimer>
#include "application.h"
#include "Project/project.h"
#include "DatabaseView/SongDatabaseView/songtableview.h"
#include "Database/database.h"
#include <QSplitter>
#include "AttachmentView/attachmentchooser.h"
#include <QHBoxLayout>
#include "Database/SongDatabase/songdatabasesortproxy.h"


SongDatabaseWidget::SongDatabaseWidget(QWidget *parent) :
    DatabaseWidget(new SongTableView(), new AttachmentChooser(), parent)
{
    databaseView()->setModel( app().project()->songDatabaseProxy() );

    connect( databaseView()->selectionModel(),
             SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
             this,
             SLOT(updateAttachmentChooser()) );
}

void SongDatabaseWidget::updateAttachmentChooser()
{
    QModelIndexList list = databaseView()->selectionModel()->selectedRows();

    if (list.isEmpty())
    {
        attachmentChooser()->setSong( NULL );
    }
    else
    {
        Song* song = databaseView()->model()->resolveItemAtIndex( list.first() );
        attachmentChooser()->setSong( song );
    }
}

AttachmentChooser* SongDatabaseWidget::attachmentChooser() const
{
    return m_secondWidget;
}
