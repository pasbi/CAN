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


SongDatabaseWidget::SongDatabaseWidget(QWidget *parent) :
    QWidget(parent)
{

    setupUi();

    m_tableViewContainer->setModel( app().project()->songDatabaseProxy() );

    connect( m_tableViewContainer->songTableView()->selectionModel(),
             SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
             this,
             SLOT(updateAttachmentChooser()) );

}

SongDatabaseWidget::~SongDatabaseWidget()
{
}

void SongDatabaseWidget::setupUi()
{
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    m_tableViewContainer = new SongTableViewContainer(this);   //TODO TableViewContainer<Song>
    m_attachmentChooser = new AttachmentChooser(splitter);
    splitter->addWidget(m_tableViewContainer);
    splitter->addWidget(m_attachmentChooser);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(splitter);
    setLayout(layout);
}

Song* SongDatabaseWidget::currentSong() const
{
    return m_tableViewContainer->currentSong();
}

void SongDatabaseWidget::updateAttachmentChooser()
{
    QModelIndexList list = m_tableViewContainer->songTableView()->selectionModel()->selectedRows();

    if (list.isEmpty())
    {
        m_attachmentChooser->setSong( NULL );
    }
    else
    {
        if (list.first() != m_currentIndex)
        {
            m_attachmentChooser->setSong(
                            m_tableViewContainer->songTableView()->model()->resolveItemAtIndex( list.first() ) );
        }
    }
}

AttachmentChooser* SongDatabaseWidget::attachmentChooser() const
{
    return m_attachmentChooser;
}

SongTableView* SongDatabaseWidget::songTableView() const
{
    return m_tableViewContainer->songTableView();
}
