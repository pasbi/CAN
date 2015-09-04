#include "songdatabasewidget.h"
#include <QTimer>
#include "application.h"
#include "Project/project.h"
#include "DatabaseView/SongDatabaseView/songtableview.h"
#include "Database/database.h"
#include <QSplitter>
#include "songtableviewcontainer.h"
#include "AttachmentView/attachmentchooser.h"
#include <QHBoxLayout>
#include "Database/SongDatabase/songdatabasesortproxy.h"


SongDatabaseWidget::SongDatabaseWidget(QWidget *parent) :
    DatabaseWidget(new SongTableViewContainer(), new AttachmentChooser(), parent)
{
    m_databaseViewContainer->setDatabase( app().project()->songDatabaseProxy() );

    connect( m_databaseViewContainer->databaseView()->selectionModel(),
             SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
             this,
             SLOT(updateAttachmentChooser()) );
}

void SongDatabaseWidget::updateAttachmentChooser()
{
    QModelIndexList list = m_databaseViewContainer->databaseView()->selectionModel()->selectedRows();

    if (list.isEmpty())
    {
        attachmentChooser()->setSong( NULL );
    }
    else
    {
        //TODO m_currentIndex seems to be never written
        if (list.first() != m_currentIndex)
        {
            attachmentChooser()->setSong(
                            m_databaseViewContainer->databaseView()->model()->resolveItemAtIndex( list.first() ) );
        }
    }
}

AttachmentChooser* SongDatabaseWidget::attachmentChooser() const
{
    return m_secondWidget;
}
