#ifndef ATTACHMENTCHOOSER_H
#define ATTACHMENTCHOOSER_H

#include <QWidget>
#include "Database/SongDatabase/song.h"

namespace Ui {
class AttachmentChooser;
}

class AttachmentChooser : public QWidget
{
    Q_OBJECT

public:
    explicit AttachmentChooser(QWidget *parent = 0);
    ~AttachmentChooser();
    int currentAttachmentIndex() const;
    Attachment* currentAttachment() const;

public slots:
    void setSong( Song* song );
    Song* song() const { return m_song; }
    void setAttachment(int index );
    void updateAttachmentView();

private slots:
    void on_pushButton_clicked();

private:
    Ui::AttachmentChooser *ui;
    Song* m_song;
    Attachment* m_currentAttachment = NULL;
    QMap<Song*, int> m_lastOpenedIndex;

};

#endif // ATTACHMENTCHOOSER_H
