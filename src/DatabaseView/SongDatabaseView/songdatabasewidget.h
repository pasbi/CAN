#ifndef SONGDATABASEWIDGET_H
#define SONGDATABASEWIDGET_H

#include <QWidget>
#include "Database/SongDatabase/songdatabase.h"
#include "Database/SongDatabase/songdatabasesortproxy.h"
#include "AttachmentView/attachmentchooser.h"


namespace Ui {
class SongDatabaseWidget;
}

class SongTableView;
class SongDatabaseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SongDatabaseWidget(QWidget *parent = 0);
    ~SongDatabaseWidget();
    void setSongDatabase( SongDatabase * songDatabase );
    SongDatabase* songDatabase() const { return m_songDatabase; }
    Song* currentSong() const;

    AttachmentChooser* attachmentChooser() const;
    SongTableView* tableView() const;

private slots:
    void updateAttachmentChooser();

private:
    Ui::SongDatabaseWidget *ui;
    SongDatabase* m_songDatabase;
    SongDatabaseSortProxy m_sortFilterProxy;
    QModelIndex m_currentIndex;


};
#endif // SONGDATABASEWIDGET_H
