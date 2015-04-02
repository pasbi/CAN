#include "songdatabasewidget.h"
#include "ui_songdatabasewidget.h"
#include <QTimer>

SongDatabaseWidget::SongDatabaseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SongDatabaseWidget)
{
    ui->setupUi(this);
    connect( ui->songTableViewContainer->songTableView(), SIGNAL(pressed(QModelIndex)), this, SLOT(updateAttachmentChooser()) );
}

SongDatabaseWidget::~SongDatabaseWidget()
{
    delete ui;
}

Song* SongDatabaseWidget::currentSong() const
{
    return ui->songTableViewContainer->currentSong();
}

void SongDatabaseWidget::setSongDatabase( SongDatabase * songDatabase )
{
    m_songDatabase = songDatabase;
    m_sortFilterProxy.setSourceModel( songDatabase );
    ui->songTableViewContainer->setModel( &m_sortFilterProxy );

}

void SongDatabaseWidget::updateAttachmentChooser()
{
    QModelIndexList list = ui->songTableViewContainer->songTableView()->selectionModel()->selectedRows();

    if (list.isEmpty())
    {
        ui->attachmentChooser->setSong( NULL );
    }
    else
    {
        if (list.first() != m_currentIndex)
        {
            m_currentIndex = list.first();
            ui->attachmentChooser->setSong(
                            ui->songTableViewContainer->songTableView()->model()->songAtIndex( m_currentIndex ) );
        }
    }
}

AttachmentChooser* SongDatabaseWidget::attachmentChooser() const
{
    return ui->attachmentChooser;
}

SongTableView* SongDatabaseWidget::tableView() const
{
    return ui->songTableViewContainer->songTableView();
}
