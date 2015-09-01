#ifndef SONGDATABASEWIDGET_H
#define SONGDATABASEWIDGET_H

#include <QWidget>
#include <QModelIndex>

namespace Ui {
class SongDatabaseWidget;
}

class SongTableView;
class SongDatabase;
class AttachmentChooser;
class Song;
class SongDatabaseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SongDatabaseWidget(QWidget *parent = 0);
    ~SongDatabaseWidget();
    Song* currentSong() const;

    AttachmentChooser* attachmentChooser() const;
    SongTableView* songTableView() const;

public slots:
    void updateAttachmentChooser();

private:
    Ui::SongDatabaseWidget *ui;
    QModelIndex m_currentIndex;


};
#endif // SONGDATABASEWIDGET_H
