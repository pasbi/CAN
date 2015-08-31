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
    void renameCurrentAttachment();

private slots:
    void editTags();
    void focusAttachment( const Attachment* a );

private:
    Ui::AttachmentChooser *ui;
    Song* m_song;
    Attachment* m_currentAttachment = NULL;
    QMap<Song*, int> m_lastOpenedIndex;
    QAction* m_editTagAction;

protected:
    bool eventFilter(QObject *o, QEvent *e);

};

#endif // ATTACHMENTCHOOSER_H
