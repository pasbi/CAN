#ifndef SONGDATABASEWIDGET_H
#define SONGDATABASEWIDGET_H

#include "DatabaseView/databasewidget.h"
#include <QModelIndex>

class Song;
class AttachmentChooser;
class SongDatabaseWidget : public DatabaseWidget<Song>
{
    Q_OBJECT

public:
    explicit SongDatabaseWidget(QWidget *parent = 0);
    AttachmentChooser* attachmentChooser() const;

protected:
    void setupUi();

public slots:
    void updateAttachmentChooser();

public:
    QModelIndex m_currentIndex;
    AttachmentChooser* m_attachmentChooser;


};
#endif // SONGDATABASEWIDGET_H
