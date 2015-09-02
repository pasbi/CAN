#ifndef SONGDATABASEWIDGET_H
#define SONGDATABASEWIDGET_H

#include <QWidget>
#include <QModelIndex>

class SongTableView;
class SongDatabase;
class AttachmentChooser;
class Song;
class SongTableViewContainer;
class AttachmentChooser;
class SongDatabaseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SongDatabaseWidget(QWidget *parent = 0);
    ~SongDatabaseWidget();
    Song* currentSong() const;

    AttachmentChooser* attachmentChooser() const;
    SongTableView* songTableView() const;

protected:
    void setupUi();

public slots:
    void updateAttachmentChooser();

public:
    QModelIndex m_currentIndex;

    SongTableViewContainer* m_tableViewContainer;
    AttachmentChooser* m_attachmentChooser;


};
#endif // SONGDATABASEWIDGET_H
