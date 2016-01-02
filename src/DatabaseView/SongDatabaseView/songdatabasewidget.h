#ifndef SONGDATABASEWIDGET_H
#define SONGDATABASEWIDGET_H

#include "DatabaseView/databasewidget.h"
#include <QModelIndex>

class Song;
class AttachmentChooser;
class SongDatabaseWidget : public DatabaseWidget<Song, AttachmentChooser>
{
    Q_OBJECT

public:
    explicit SongDatabaseWidget(QWidget *parent = 0);
    AttachmentChooser* attachmentChooser() const;

public slots:
    void updateAttachmentChooser();


};
#endif // SONGDATABASEWIDGET_H
