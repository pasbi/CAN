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
    DatabaseWidget<Song>(parent)
{
    setupUi();
    m_databaseViewContainer->setDatabase( app().project()->songDatabaseProxy() );

    connect( m_databaseViewContainer->databaseView()->selectionModel(),
             SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
             this,
             SLOT(updateAttachmentChooser()) );
}


void SongDatabaseWidget::setupUi()
{
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    m_databaseViewContainer = new SongTableViewContainer(this);
    m_attachmentChooser = new AttachmentChooser(splitter);
    splitter->addWidget(m_databaseViewContainer);
    splitter->addWidget(m_attachmentChooser);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(splitter);
    setLayout(layout);
}

void SongDatabaseWidget::updateAttachmentChooser()
{
    QModelIndexList list = m_databaseViewContainer->databaseView()->selectionModel()->selectedRows();

    if (list.isEmpty())
    {
        m_attachmentChooser->setSong( NULL );
    }
    else
    {
        // todo m_currentIndex seems to be never written
        if (list.first() != m_currentIndex)
        {
            m_attachmentChooser->setSong(
                            m_databaseViewContainer->databaseView()->model()->resolveItemAtIndex( list.first() ) );
        }
    }
}

AttachmentChooser* SongDatabaseWidget::attachmentChooser() const
{
    return m_attachmentChooser;
}
