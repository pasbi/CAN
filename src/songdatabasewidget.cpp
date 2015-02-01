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

void SongDatabaseWidget::setSongDatabase( SongDatabase * songDatabase )
{
    m_songDatabase = songDatabase;
    // connect currentSongChanged ui->attachmentChooser --> set Song

    m_sortFilterProxy.setSourceModel( songDatabase );
    ui->songTableViewContainer->setModel( &m_sortFilterProxy );
}
