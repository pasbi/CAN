#include "songdatabasewidget.h"
#include "ui_songdatabasewidget.h"

SongDatabaseWidget::SongDatabaseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SongDatabaseWidget)
{
    ui->setupUi(this);
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

    connect( ui->songTableViewContainer->songTableView()->selectionModel(),
             SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
             this,
             SLOT( updateAttachmentChooser(QModelIndex, QModelIndex) ));

}

void SongDatabaseWidget::updateAttachmentChooser(QModelIndex currentRow, QModelIndex previousRow)
{
    Q_UNUSED( previousRow );
    Song* song = ui->songTableViewContainer->songTableView()->model()->songAtIndex( currentRow );
    ui->attachmentChooser->setSong( song );
}

AttachmentChooser* SongDatabaseWidget::attachmentChooser() const
{
    return ui->attachmentChooser;
}

SongTableView* SongDatabaseWidget::tableView() const
{
    return ui->songTableViewContainer->songTableView();
}
